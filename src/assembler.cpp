#include "assembler.h"
#include "codedata.h"
#include "vm_opcode.h"
#include "vm.h"
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define GET_NAME_MAP(NAME) {#NAME,GET_OP_NAME(NAME)},
const int OP_LOAD=0x12345678;
map<string,int> opcode={
    {"LOAD",OP_LOAD},
    OP_CODE(GET_NAME_MAP)
};

CODE gcode(OPCODE code)
{
    return code<<26;
}

CODE gcode(OPCODE code,int a)
{
    return gcode(code)|(a&0x3ffffff);
}

CODE gcode(OPCODE code,int a,int b)
{
    return gcode(code)|((a<<13)&0x3ffe000)|(b&0x1fff);
}


bool get_asm_line(ifstream &stream,string &s)
{
    string str;
    if(!getline(stream,str,'\n'))return false;
    istringstream ss(str);
    getline(ss,s,';');
    if(s.empty())return get_asm_line(stream,s);
    for(int i=0;i<s.length();i++)if(s[i]>='a'&&s[i]<='z')s[i]-=32;
    return true;
}

enum OPND_TYPE
{
    OT_INT,OT_NULL,OT_TRUE,OT_FALSE,OT_FLOAT,OT_STRING,OT_LABEL
};

OPND_TYPE get_opnd_type(const string &s)
{
    char b=s[0],e=*s.rbegin();
    if(b=='"'&&e=='"')return OT_STRING;
    if(s=="NULL")return OT_NULL;
    if(s=="TRUE")return OT_TRUE;
    if(s=="FALSE")return OT_FALSE;

    OPND_TYPE t=OT_INT;
    for(int i=0;i<s.length();i++)if(!isdigit(s[i])&&(i&&s[0]!='-'))
    {
        if(s[i]=='.'||(i==s.length()-1&&(s[i]=='f'||s[i]=='F')))
        {
            t=OT_FLOAT;
        }
        else
        {
            t=OT_LABEL;
            break;
        }
    }
    return t;
}

map<string,int> labels;
map<string,int> string_table;
map<FLOAT_TYPE,int> float_table;
map<INT_TYPE,int> int_table;

vector<int> int_table_list;
vector<FLOAT_TYPE> float_table_list;
vector<string> string_table_list;

bool get_opnd(const string &s,int &v)
{
    int opnd_int;
    FLOAT_TYPE opnd_float;
    string opnd_string;
    switch(get_opnd_type(s))
    {
        case OT_INT:
            opnd_int=atoi(s.c_str());
            if(!int_table.count(opnd_int))
            {
                v=int_table.size();
                int_table[opnd_int]=v;
                int_table_list.push_back(opnd_int);
            }
            else v=int_table[opnd_int];
            break;
        case OT_FLOAT:
            opnd_float=atof(s.c_str());
            if(!int_table.count(opnd_float))
            {
                v=float_table.size();
                float_table[opnd_float]=v;
                float_table_list.push_back(opnd_float);
            }
            else v=float_table[opnd_float];
            break;
        case OT_STRING:
            opnd_string=string(s,1,s.length()-2);

            if(!string_table.count(opnd_string))
            {
                v=string_table.size();
                string_table[opnd_string]=v;
                string_table_list.push_back(opnd_string);
            }
            else v=int_table[opnd_int];
            break;
        case OT_LABEL:
            if(!labels.count(s))return false;
            v=labels[s];
            break;
    }
    return true;
}
int get_opnd_int(const string &s)
{
    return atoi(s.c_str());
}
double get_opnd_float(const string &s)
{
    return atof(s.c_str());
}
string get_opnd_string(const string &s)
{
    return string(s,1,s.length()-2);
}

struct S_CODE
{
    int line;
    string oper;
    vector<string> opnd;
};
vector<S_CODE> s_code;
bool load_asmfile(const char *filename)
{
    ifstream asms(filename);
    if(!asms)return false;
    string line;
    for(int lines=1;get_asm_line(asms,line);lines++)
    {
        istringstream s(line);
        string oper;
        getline(s,oper,' ');
        string t;
        vector<string> opnd;
        while(getline(s,t,','))opnd.push_back(t);
        if(opnd.empty()&&*oper.rbegin()==':')labels[string(oper,0,oper.size()-1)]=s_code.size();
        else s_code.push_back({lines,oper,opnd});
    }
    asms.close();
    return true;
}

vector<CODE> code;
bool gen_code()
{
    for(auto &x:s_code)
    {
        if(!opcode.count(x.oper))
        {
            fprintf(stderr,"Error(%d): unknown instruction \"%s\"\n",x.line,x.oper.c_str());
            return false;
        }
        int op=opcode[x.oper];
        if(op==OP_LOAD)
        {

            if(x.opnd.size()!=2)
            {
                fprintf(stderr,"Error(%d): too much operand\n",x.line);
                return false;
            }
            switch(get_opnd_type(x.opnd[1]))
            {
                case OT_NULL:
                    code.push_back(gcode(OP_NLOAD));
                    continue;
                case OT_TRUE:
                    code.push_back(gcode(OP_BLOAD,1));
                    continue;
                case OT_FALSE:
                      code.push_back(gcode(OP_BLOAD,0));
                    continue;
                case OT_INT:
                    op=OP_ILOAD;
                    break;
                case OT_FLOAT:
                    op=OP_FLOAD;
                    break;
                case OT_STRING:
                    op=OP_SLOAD;
                    break;
                default:
                    fprintf(stderr,"Error(%d): incorrect operand\n",x.line);
                    return false;
            }
            int v0=get_opnd_int(x.opnd[0]),v1;
            get_opnd(x.opnd[1],v1);
            code.push_back(gcode((OPCODE)op,v0,v1));
            continue;
        }
        int opnds[2];
        for(int i=0;i<x.opnd.size();i++)
        {
            switch(get_opnd_type(x.opnd[i]))
            {
                case OT_INT:
                    opnds[i]=get_opnd_int(x.opnd[i]);
                    break;
                case OT_LABEL:
                    if(!get_opnd(x.opnd[i],opnds[i]))return fprintf(stderr,"Error(%d): cannot found label \"%s\"\n",x.line,x.opnd[i].c_str()),false;
                    break;
                default:
                fprintf(stderr,"Error(%d): incorrect operand, operand must be integer or label\n",x.line);
                return false;
            }
        }
        switch(x.opnd.size())
        {
            case 0:
                code.push_back(gcode((OPCODE)op));
                break;
            case 1:
                code.push_back(gcode((OPCODE)op,opnds[0]));
                break;
            case 2:
                code.push_back(gcode((OPCODE)op,opnds[0],opnds[1]));
                break;
            default:
                fprintf(stderr,"Error(%d): too much operand\n",x.line);
                return false;
        }
    }
    return true;
}

bool generate_bin(const char *filename)
{
    FILE *fp=fopen(filename,"wb");
    if(!fp)return false;
    CodeConfig code_config;
    code_config.magic=HEADER_MAGIC;
    code_config.int_table_size=int_table.size();
    code_config.float_table_size=float_table.size();
    code_config.string_table_size=string_table.size();
    code_config.string_raw_size=0;
    code_config.entry_point=labels["MAIN"];
    code_config.code_raw_size=code.size();
    for(int i=0;i<string_table_list.size();i++)code_config.code_raw_size+=string_table_list[i].length()+1;
    fwrite(&code_config,sizeof(code_config),1,fp);

    // int table
    fwrite(&int_table_list[0],sizeof(int_table_list[0]),int_table_list.size(),fp);
    // float table
    fwrite(&float_table_list[0],sizeof(float_table_list[0]),float_table_list.size(),fp);
    size_t v=0;
    fwrite(&v,sizeof(v),1,fp); //string hack for vm
    //string table
    for(int i=0;i<string_table_list.size();i++)
    {
        string &s=string_table_list[i];
        fwrite(s.c_str(),1,s.length()+1,fp);
    }
    //code
    fwrite(&code[0],sizeof(code[0]),code.size(),fp);
    fclose(fp);
    return true;
}


bool assemble(const char *asmfile,const char *outfile)
{
    if(!load_asmfile(asmfile))return false;
    if(!gen_code())return false;
    return generate_bin(outfile);
}