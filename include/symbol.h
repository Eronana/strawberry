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
class Symbol
{
private:
    typedef map<string,int> map_si;
    map_si table;
    map_si globalTable;
    Symbol *parent;
public:
    static const int EMPTY_SYMBOL;
    int allowIndex;
    const SCOPENAME scope;
    int beginIndex;
    Symbol(Symbol *parent,SCOPENAME scope);
    void print(FILE *fp);
    bool insert(const string &str);
    int lookup(const string &str);
    bool matchScope(SCOPENAME s);
    Symbol *getParent();
    static bool isEmpty(int index);
    static bool isGlobal(int index);
    static int getGlobalIndex(int index);
};
#endif
