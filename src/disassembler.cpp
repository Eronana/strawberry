#include "disassembler.h"
#include "codedata.h"
#include "vm_opcode.h"
#include "vm.h"

#define GET_NAME_LIST(NAME,LENGTH) #NAME,
#define GET_OP_LENGTH_LIST(NAME,LENGTH) LENGTH,
const char *op_name[]={OP_CODE(GET_NAME_LIST)};
const size_t op_length[]={OP_CODE(GET_OP_LENGTH_LIST)};

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
void discode(CODE code,CodeData &code_data,FILE *out)
{
    OPCODE op=get_ins(code);
    switch(op)
    {
        case OP_NLOAD:
            fprintf(out,"LOAD NULL\n");
            break;
        case OP_BLOAD:
            fprintf(out,"LOAD %d,%s\n",get_a(code),get_b(code)?"TRUE":"FALSE");
            break;
        case OP_ILOAD:
            fprintf(out,"LOAD %d,%d\n",get_a(code),code_data.int_table[get_b(code)]);
            break;
        case OP_FLOAD:
            fprintf(out,"LOAD %d,%f\n",get_a(code),code_data.float_table[get_b(code)]);
            break;
        case OP_SLOAD:
            fprintf(out,"LOAD %d,",get_a(code));
            print_string(out,code_data.string_table[get_b(code)]);
            fputc('\n',out);
            break;
        default:
            if(op>=SIZEOF(op_name))
            {
                fprintf(out,"; unknow instructor: %x\n",code);
                break;
            }
            fputs(op_name[op],out);
            switch(op_length[op])
            {
                case 0:
                    putchar('\n');
                    break;
                case 1:
                    fprintf(out," %d\n",get_A(code));
                    break;
                case 2:
                    fprintf(out," %d,%d\n",get_a(code),get_b(code));
            }
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
    for(int i=0;i<code_data.config.code_raw_size;i++)discode(code_data.code_raw[i],code_data,out);
    return true;
}
