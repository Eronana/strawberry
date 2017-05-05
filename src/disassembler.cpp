#include "disassembler.h"
#include "codedata.h"
#include "vm_opcode.h"
#include "vm.h"

#define GET_NAME(NAME,LEN) #NAME,
const char *op_name[]={OP_CODE(GET_NAME)};


bool disassemble(const char *binfile,const char *outfile)
{
    FILE *in=fopen(binfile,"rb");
    if(!in)return false;
    FILE *out=fopen(outfile,"w");
    if(!out)
    {
        fclose(in);
        return false;
    }
    bool ret=disassemble(in,out);
    fclose(in);
    fclose(out);
    return ret;
}
char r_escape_table[128];
void init_r_escape_table()
{
    if(r_escape_table['\0'])return;
    r_escape_table['\0']='0';
    r_escape_table['\e']='e';
    r_escape_table['\a']='a';
    r_escape_table['\b']='b';
    r_escape_table['\t']='t';
    r_escape_table['\n']='n';
    r_escape_table['\v']='v';
    r_escape_table['\f']='f';
    r_escape_table['\r']='r';
}
void print_string(FILE *fp,const char *s)
{
    init_r_escape_table();
    fputc('"',fp);
    for(int i=0;s[i];i++)
    {
        if(r_escape_table[s[i]])fputc('\\',fp),fputc(r_escape_table[s[i]],fp);
        else fputc(s[i],fp);
    }
    fputc('"',fp);
}
int8_t get_int8(CODE *code)
{
    return *(int8_t*)(code+1);
}

int32_t get_int32(CODE *code)
{
    return *(int32_t*)(code+1);
}


void discode(CODE &op,CodeData &code_data,FILE *out)
{
    switch(op)
    {
        case OP_NPUSH:
            fprintf(out,"PUSH NULL\n");
            break;
        case OP_BPUSH:
            fprintf(out,"PUSH %s\n",get_int8(&op)?"TRUE":"FALSE");
            break;
        case OP_IPUSH:
            fprintf(out,"PUSH %d\n",code_data.int_table[get_int32(&op)]);
            break;
        case OP_FPUSH:
            fprintf(out,"PUSH %f\n",code_data.float_table[get_int32(&op)]);
            break;
        case OP_SPUSH:
            fprintf(out,"PUSH ");
            print_string(out,code_data.string_table[get_int32(&op)]);
            fputc('\n',out);
            break;
        case OP_LOAD_GLOBAL:
        case OP_STORE_GLOBAL:
        case OP_LOAD:
        case OP_STORE:
        case OP_LOAD_EXTERNAL:
        case OP_STORE_EXTERNAL:
        case OP_CREATE_FUNCTION:
        case OP_ADDSP:
        case OP_SUBSP:
        case OP_CALL:
        case OP_JMP:

            fprintf(out,"%s %d\n",op_name[op],get_int32(&op));
            break;
        default:
            if(op>=SIZEOF(op_name))
            {
                fprintf(out,"; unknow instructor: %x\n",op);
                break;
            }
            fprintf(out,"%s\n",op_name[op]);
    }
}

bool disassemble(FILE *in,FILE *out)
{
    CodeData code_data;
    if(!code_data.load(in))return false;
    fprintf(out,"; ======[   int table  ]======\n");
    for(int i=0;i<code_data.config.int_table_size;i++)fprintf(out,"; %3d | %d\n",i,code_data.int_table[i]);
    fprintf(out,"; ======[  float table ]======\n");
    for(int i=0;i<code_data.config.float_table_size;i++)fprintf(out,"; %3d | %f\n",i,code_data.float_table[i]);
    fprintf(out,"; ======[ string table ]======\n");
    for(int i=0;i<code_data.config.string_table_size;i++)fprintf(out,"; %3d | %s\n",i,code_data.string_table[i]);
    fprintf(out,"; ======[     code     ]======\n");
    for(int i=0;i<code_data.config.code_raw_size;i+=op_length[code_data.code_raw[i]])
    {
        discode(code_data.code_raw[i],code_data,out);
    }
    return true;
}
