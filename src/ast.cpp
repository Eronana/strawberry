#include "ast.h"
#include <cstdio>

AST::~AST(){}
// Literal
AST_NAME(Literal)::AST_NAME(Literal)(const Token &token):token(token){}
AST_NAME(Literal)::AST_NAME(Literal)(int line,int col)
{
    token.type=TOKEN_NULL_LITERAL;
    token.line=line;
    token.col=col;
    token.raw="null";
}

map<AST*,unique_ptr<Symbol>> AST::scope_map;
int AST::sp=0;
FILE *fp=stdout;
int this_count=0;

void AST::init()
{
    scope_map.clear();
    sp=0;
    this_count=0;
}
