#include "symbol.h"
#include "ast.h"
#include "ast_method.h"
#include <cstdio>
const int Symbol::EMPTY_SYMBOL=0x7fffffff;

Symbol::Symbol(Symbol *parent,SCOPENAME scope):parent(parent),scope(scope)
{
    if(scope==SCOPE_GLOBAL||scope==SCOPE_FUNCTION)allowIndex=0;
    else allowIndex=parent->allowIndex;
    localCount=0;
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
bool Symbol::insert(const std::string &str,int idx)
{
    if(!table.insert(std::make_pair(str,idx)).second)return false;
    return true;
}
Symbol *Symbol::getParent()
{
    return parent;
}
SYMTYPE Symbol::lookup(const string &str,int &idx)
{
    if(table.count(str))
    {
        idx=table[str];
        if(scope==SCOPE_GLOBAL)return ST_GLOBAL;
        else return ST_LOCAL;
    }
    if(parent)
    {
        if(scope==SCOPE_FUNCTION)
        {
            auto ret=parent->lookup(str,idx);
            if(ret!=ST_EMPTY)ret=ST_EXTERNAL;
            return ret;
        }
        else return parent->lookup(str,idx);
    }
    return ST_EMPTY;
}
bool Symbol::matchScope(SCOPENAME s)
{
    if(scope==s)return true;
    if(!parent||scope==SCOPE_FUNCTION)return false;
    return parent->matchScope(s);
}

void Symbol::setParentCount(int count)
{
    if(scope==SCOPE_GLOBAL||scope==SCOPE_FUNCTION)
    {
        if(count>localCount)localCount=count;
    }
    else parent->setParentCount(count);
}

void Symbol::setParentCount()
{
    setParentCount(allowIndex);
}
