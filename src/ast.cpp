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

