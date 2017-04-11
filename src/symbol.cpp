#include "symbol.h"
#include "ast.h"
#include <cstdio>
const int Symbol::EMPTY_SYMBOL=0x7fffffff;

Symbol::Symbol(Symbol *parent,SCOPENAME scope):parent(parent),scope(scope)
{
    if(scope==SCOPE_GLOBAL||scope==SCOPE_FUNCTION||parent->scope==SCOPE_GLOBAL)allowIndex=0;
    else allowIndex=parent->allowIndex;
}

void Symbol::print()
{
    for(auto &x:table)
    {
        fprintf(AST::fp,"%s: %d\n",x.first.c_str(),x.second);
    }
    if(parent)parent->print();
}
bool Symbol::insert(const std::string &str)
{
    if(!table.insert(std::make_pair(str,allowIndex)).second)return false;
    allowIndex++;
    return true;
}
Symbol *Symbol::getParent()
{
    return parent;
}
int Symbol::lookup(const std::string &str)
{
    if(table.count(str))
    {
        if(scope==SCOPE_GLOBAL)return ~table[str];
        return table[str];
    }
    if(parent)return parent->lookup(str);
    return EMPTY_SYMBOL;
}
bool Symbol::matchScope(SCOPENAME s)
{
    if(scope==s)return true;
    if(!parent)return false;
    return parent->matchScope(s);
}
bool Symbol::isEmpty(int index)
{
    return index==EMPTY_SYMBOL;
}
bool Symbol::isGlobal(int index)
{
    return index<0;
}
int Symbol::getGlobalIndex(int index)
{
    return ~index;
}

