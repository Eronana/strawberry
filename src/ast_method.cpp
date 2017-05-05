#include "ast_method.h"

map<AST*,unique_ptr<Symbol>> scope_map;
stack<int> continueStack;
stack<int> breakStack;
stack<int> defaultClauseStack;
stack<FunctionInfo*> functionStack;

int sp=0;
int this_count=0;
int next_label=0;

void astMethodInit()
{
    scope_map.clear();
    continueStack={};
    breakStack={};
    defaultClauseStack={};
    functionStack={};
    sp=0;
    this_count=0;
    next_label=0;
}

int nextLabel()
{
    return next_label++;
}
