#include "lexer.h"
#include <cstdio>

#define TEST_SOURCE_FILE "test/data/lexer.test.st"

int main()
{
    Lexer lexer;
    if(!lexer.load(TEST_SOURCE_FILE))
    {
        fprintf(stderr,"cannot open file: %s\n",TEST_SOURCE_FILE);
        return 1;
    }
    for(Token t;(t=lexer.next()).type!=TOKEN_EOF;t.print());
    return 0;
}