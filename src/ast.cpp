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
stack<int> continueStack;
stack<int> breakStack;
FILE *fp=stdout;
int AST::sp=0;
int this_count=0;
int next_label=0;

void AST::init()
{
    scope_map.clear();
    continueStack={};
    breakStack={};
    sp=0;
    this_count=0;
    next_label=0;
}

int AST::nextLabel()
{
    return next_label++;
}
