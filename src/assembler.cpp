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

#define MAX_INT8 0x7f
#define MIN_INT8 -0x80
#define MAX_INT16 0x7fff
#define MIN_INT16 -0x8000
#define MAX_INT32 0x7fffffff
#define MIN_INT32 -0x80000000

#define GET_NAME_MAP(NAME,LEN) {#NAME,GET_OP_NAME(NAME,LEN)},
const size_t op_length[]={OP_CODE(GET_OP_LENGTH)};
char escape_table[128];

const int OP_PUSH=0x12345678;
map<string,int> opcode={
    {"PUSH",OP_PUSH},
    OP_CODE(GET_NAME_MAP)
};


int get_asm_line(ifstream &stream,string &s)
{
    string str;
    if(!getline(stream,str,'\n'))return -0x80000000;
    istringstream ss(str);
    getline(ss,s,';');
    if(s.empty())return 1+get_asm_line(stream,s);
    return true;
}

enum OPND_TYPE
{
    OT_INT,OT_NULL,OT_BOOL,OT_FLOAT,OT_STRING,OT_LABEL
};

OPND_TYPE get_opnd_type(const string &s)
{
    char b=s[0],e=*s.rbegin();
    if((b=='"'&&e=='"')||(b=='\''&e=='\''))return OT_STRING;
    if(s=="NULL")return OT_NULL;
    if(s=="TRUE")return OT_BOOL;
    if(s=="FALSE")return OT_BOOL;

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
vector<pair<int,int>> jmp_table;
map<int,int> op_pos;
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
        case OT_BOOL:
            opnd_int=s[0]=='T'?1:0;
            if(!int_table.count(opnd_int))
            {
                v=int_table.size();
                int_table[opnd_int]=v;
                int_table_list.push_back(opnd_int);
            }
            else v=int_table[opnd_int];
            break;
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
    string opnd;
};

vector<S_CODE> s_code;
bool load_asmfile(const char *filename)
{
    ifstream asms(filename);
    if(!asms)return false;
    string line;
    for(int lines=0;;)
    {
        int read_lines=get_asm_line(asms,line);
        if(read_lines<0)break;
        lines+=read_lines;
        int pos=line.find(' ');
        string oper=line.substr(0,pos);
        toUpperCase(oper);
        string opnd;
        if(pos!=string::npos)
        {
            opnd=line.substr(pos+1);
            if(opnd.length()&&opnd[0]!='\''&&opnd[0]!='"')toUpperCase(opnd);
        }
        if(opnd.empty()&&*oper.rbegin()==':')
        {
            string label=string(oper,0,oper.size()-1);
            labels[toUpperCase(label)]=s_code.size();
        }
        else
        {
            s_code.push_back({lines,oper,opnd});
        }
    }
    asms.close();
    return true;
}

vector<CODE> code;
void add_int8(int8_t a)
{
    code.push_back((int8_t)a);
}

void add_int32(int32_t a)
{
    auto size=code.size();
    code.resize(size+4);
    *(int32_t*)&code[size]=a;
}
bool gen_code()
{
    int op_code_size=0;
    for(auto &x:s_code)
    {
        op_pos[op_pos.size()]=op_code_size;
        if(!opcode.count(x.oper))
        {
            fprintf(stderr,"Error(line %d): unknown instruction \"%s\"\n",x.line,x.oper.c_str());
            return false;
        }
        int op=opcode[x.oper];
        int int_opnd=-1;
        if(op==OP_PUSH)
        {
            switch(get_opnd_type(x.opnd))
            {
                case OT_NULL:
                    op=OP_NPUSH;
                    break;
                case OT_BOOL:
                    op=OP_BPUSH;
                    break;
                case OT_INT:
                    op=OP_IPUSH;
                    break;
                case OT_FLOAT:
                    op=OP_FPUSH;
                    break;
                case OT_STRING:
                    op=OP_SPUSH;
                    break;
                default:
                    fprintf(stderr,"Error(line %d): incorrect operand\n",x.line);
                    return false;
            }
            get_opnd(x.opnd,int_opnd);
        }
        code.push_back(op);
        switch(op)
        {
            case OP_IPUSH:
            case OP_FPUSH:
            case OP_SPUSH:
            case OP_LOAD_GLOBAL:
            case OP_STORE_GLOBAL:
            case OP_LOAD:
            case OP_STORE:
            case OP_LOAD_EXTERNAL:
            case OP_STORE_EXTERNAL:
            case OP_ADDSP:
            case OP_SUBSP:
            case OP_CALL:
                if(int_opnd==-1)int_opnd=atoi(x.opnd.c_str());
                add_int32(int_opnd);
                break;
            case OP_CREATE_FUNCTION:
            case OP_JMP:
                jmp_table.push_back({code.size(),labels[x.opnd]});
                add_int32(0);
                break;
            case OP_BPUSH:
                add_int8(x.opnd[0]=='T'?1:0);
                break;
        }
        op_code_size+=op_length[op];
    }
    for(auto &x:jmp_table)
    {
        *(int32_t*)&code[x.first]=op_pos[x.second];
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