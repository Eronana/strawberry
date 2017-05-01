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

#define GET_NAME_MAP(NAME,LENGTH) {#NAME,GET_OP_NAME(NAME,LENGTH)},
char escape_table[128];

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
    return true;
}

enum OPND_TYPE
{
    OT_INT,OT_NULL,OT_TRUE,OT_FALSE,OT_FLOAT,OT_STRING,OT_LABEL
};

OPND_TYPE get_opnd_type(const string &s)
{
    char b=s[0],e=*s.rbegin();
    if((b=='"'&&e=='"')||(b=='\''&e=='\''))return OT_STRING;
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
map<INT_TYPE,int> int_table={
    {0,0},
    {1,1}
};

vector<int> int_table_list={0,1};
vector<FLOAT_TYPE> float_table_list;
vector<string> string_table_list;

void init_escape_table()
{
    if(escape_table[1])return;
    for(int i=0;i<sizeof(escape_table);i++)escape_table[i]=i;
    escape_table['0']='\0';
    escape_table['e']='\e';
    escape_table['a']='\a';
    escape_table['b']='\b';
    escape_table['t']='\t';
    escape_table['n']='\n';
    escape_table['v']='\v';
    escape_table['f']='\f';
    escape_table['r']='\r';
}
string process_string(const string &s)
{
    string x;
    for(int i=1;i<s.length()-1;i++)
        x.push_back(s[i]=='\\'?escape_table[s[++i]]:s[i]);
    return x;
}
bool get_opnd(const string &s,int &v)
{
    int opnd_int;
    FLOAT_TYPE opnd_float;
    string opnd_string;
    init_escape_table();
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
            if(!float_table.count(opnd_float))
            {
                v=float_table.size();
                float_table[opnd_float]=v;
                float_table_list.push_back(opnd_float);
            }
            else v=float_table[opnd_float];
            break;
        case OT_STRING:
            opnd_string=process_string(s);
            if(!string_table.count(opnd_string))
            {
                v=string_table.size();
                string_table[opnd_string]=v;
                string_table_list.push_back(opnd_string);
            }
            else v=string_table[opnd_string];
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
string &toUpperCase(string &s)
{
    for(int i=0;i<s.length();i++)if(s[i]>='a'&&s[i]<='z')s[i]-=32;
    return s;
}
struct S_CODE
{
    int line;
    string oper;
    vector<string> opnd;
};
void get_opnds(const string &s,vector<string> &opnd)
{
    for(int i=0;i<s.length();i++)if(s[i]!=' ')
    {
        int x=i;
        if(s[i]=='"'||s[i]=='\'')
        {
            char c=s[i];
            while(!(s[i++]!='\\'&&s[i]==c));
            opnd.push_back(s.substr(x,i-x+1));
        }
        else
        {
            while(++i<s.length()&&s[i]!=',');
            opnd.push_back(s.substr(x,i-x));
            toUpperCase(*opnd.rbegin());
        }
    }
}
vector<S_CODE> s_code;
bool load_asmfile(const char *filename)
{
    ifstream asms(filename);
    if(!asms)return false;
    string line;
    for(int lines=1;get_asm_line(asms,line);lines++)
    {
        int pos=line.find(' ');
        string oper=line.substr(0,pos);
        toUpperCase(oper);
        vector<string> opnd;
        if(pos!=string::npos)get_opnds(line.substr(pos+1),opnd);
        if(opnd.empty()&&*oper.rbegin()==':')
        {
            string label=string(oper,0,oper.size()-1);
            labels[toUpperCase(label)]=s_code.size();
        }
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
            fprintf(stderr,"Error(line %d): unknown instruction \"%s\"\n",x.line,x.oper.c_str());
            return false;
        }
        int op=opcode[x.oper];
        if(op==OP_LOAD)
        {

            if(x.opnd.size()!=2)
            {
                fprintf(stderr,"Error(line %d): too much operand\n",x.line);
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
                    fprintf(stderr,"Error(line %d): incorrect operand\n",x.line);
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
                    if(!get_opnd(x.opnd[i],opnds[i]))return fprintf(stderr,"Error(line %d): cannot found label \"%s\"\n",x.line,x.opnd[i].c_str()),false;
                    break;
                default:
                fprintf(stderr,"Error(line %d): incorrect operand, operand must be integer or label\n",x.line);
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
                fprintf(stderr,"Error(line %d): too much operand\n",x.line);
                return false;
        }
    }
    return true;
}

bool generate_bin(const char *filename)
{
    return write_code_data(filename,labels["MAIN"],int_table_list,float_table_list,string_table_list,code);
}


bool assemble(const char *asmfile,const char *outfile)
{
    if(!load_asmfile(asmfile))return false;
    if(!gen_code())return false;
    return generate_bin(outfile);
}