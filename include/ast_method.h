#ifndef _AST_METHOD_H_
#define _AST_METHOD_H_

#include <stack>
#include <map>
#include "ast.h"
using namespace std;

// foreach
#define FOR2ND(B) for(int i=1;i<B.size();i++)
#define FOREACH(B) for(auto &x:B)

// print
#define PUTS(STR) fputs(STR,AST::fp)
#define PRINTF(...) fprintf(AST::fp,__VA_ARGS__)

#define GET_LITERAL(L) (((AST_NAME(Literal)*)L.get())->token)

#define GET_SCOPE() symbol=scope_map[this].get()

struct FunctionInfo
{
    map<string,int> ext_map;
    vector<int> ext_list;
};
typedef map<int,int> EXT_VAR_TYPE;
extern map<AST*,unique_ptr<Symbol>> scope_map;
extern stack<int> continueStack;
extern stack<int> breakStack;
extern stack<int> defaultClauseStack;
extern stack<FunctionInfo*> functionStack;
extern int sp;
extern int this_count;
extern int next_label;



extern int nextLabel();
extern void astMethodInit();
#endif
