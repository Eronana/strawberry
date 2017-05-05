#ifndef __SYMBOL_H_
#define __SYMBOL_H_

#include <map>
#include <string>
#include <memory>
using namespace std;

enum SCOPENAME
{
    SCOPE_GLOBAL,
    SCOPE_FUNCTION,
    SCOPE_FOR,
    SCOPE_DO,
    SCOPE_WHILE,
    SCOPE_SWITCH,
    SCOPE_BLOCK
};
enum SYMTYPE
{
    ST_GLOBAL,
    ST_EXTERNAL,
    ST_LOCAL,
    ST_EMPTY
};
class Symbol
{
private:
    Symbol *parent;
    void setParentCount(int count);
public:
    map<string,int> table;
    static const int EMPTY_SYMBOL;
    int allowIndex;
    int localCount;
    const SCOPENAME scope;
    int beginIndex;
    Symbol(Symbol *parent,SCOPENAME scope);
    void print();
    bool insert(const string &str);
    bool insert(const string &str,int idx);
    SYMTYPE lookup(const string &str,int &idx);
    bool matchScope(SCOPENAME s);
    Symbol *getParent();
    void setParentCount();
};
#endif
