#include "parser.h"
#include "lexer.h"
#include "exception.h"
#include "ast_method.h"
#include "vm.h"
#include "assembler.h"
#include "disassembler.h"

#define GETOPT(S) get_option(argc,argv,S)
#define SETOUT(S) {if(!out)out=S;}

int get_option(int argc,char *argv[],char *option)
{
    for(int i=2;i<argc;i++)if(argv[i][0]=='-'&&!strcmp(argv[i]+1,option))return i;
    return 0;
}
int main(int argc,char *argv[])
{
    char *in=argv[1];
    char *out=NULL;
    int opt;
    if(opt=GETOPT("o"))out=argv[opt+1];
    bool s=GETOPT("s"),x=GETOPT("x"),c=GETOPT("c"),d=GETOPT("d");

    if(c)
    {
        SETOUT("out.bin")
        if(assemble(in,out))return 0;
        return 1;
    }
    if(d)
    {
        SETOUT("out.asm")
        if(disassemble(in,out))return 0;
        return 1;
    }
    Lexer lexer;
    if(!lexer.load(in))
    {
        fprintf(stderr,"cannot open %s\n",in);
        return 1;
    }
    if(s)SETOUT("out.asm")
    else if(x) SETOUT("out.bin")
    else SETOUT(tmpnam(NULL))
    Parser parser(lexer);
    AST_PTR ast;
    try
    {
        ast=parser.parse();
        astMethodInit();
        ast->fill(nullptr);
        ast->check(nullptr);

        const char *asm_out;
        if(!x)asm_out=out;
        else asm_out=tmpnam(NULL);
        FILE *fp=fopen(asm_out,"w");
        AST::fp=fp;
        ast->codeGen(nullptr);
        fclose(fp);
        if(s)return 0;
        int result=assemble(asm_out,out)?0:1;
        if(x||result)
        {
            remove(asm_out);
            return result;
        }
    }
    catch(Exception &e)
    {
        e.print(stderr);
        return 1;
    }
    VirtualMachine vm;
    if(!vm.load(out))printf("cannot load %s\n",out);
    else remove(out);;
    vm.run();
    return 0;
}