#include "parser.h"
#include "lexer.h"
#include "exception.h"
#include "ast_method.h"

#define TEST_SOURCE_FILE "test/data/lexer.test.st"

int main(int argc,char *argv[])
{
    puts("======[ test parser ]======");
    const char *file=TEST_SOURCE_FILE;
    if(argc==1)printf("default file: %s\n",file);
    else file=argv[1];
    Lexer lexer;
    if(!lexer.load(file))
    {
        fprintf(stderr,"cannot open %s\n",file);
        return 1;
    }
    Parser parser(lexer);
    AST_PTR ast;
    try
    {
        ast=parser.parse();
        puts("Parsing successed!\nResult:");
        astMethodInit();
        ast->print();
        ast->fill(nullptr);
        ast->check(nullptr);
        puts("---[ asm ]----");
        ast->codeGen(nullptr);
    }
    catch(Exception &e)
    {
        e.print(stderr);
        return 1;
    }
    return 0;
}