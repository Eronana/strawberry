#include "ast.h"

#define CODEGEN(M) M->codeGen(symbol)
#define AST_CODEGEN_ARRAY() {FOREACH(nodes){CODEGEN(x);}}

/*
new_array a
    reg[a]=new Array;
    this=reg[a]

set_this a
    this=reg[a]

load a,b
    reg[a]=b

object_set a,b
    this[a]=b


*/
DEF_AST_METHOD(ArrayLiteral,AST_CODEGEN)
{
    PRINTF("new_array %d\n",sp);
    int sp_bak=sp++,this_bak=++this_count;
    int value_sp=sp,key_sp=sp+1;
    for(int i=0;i<nodes.size();i++)
    {
        CODEGEN(nodes[i]);
        if(this_bak!=this_count)
        {
            PRINTF("set_this %d\n",sp_bak);
            this_bak=this_count;
        }
        PRINTF("load %d,%d\n",key_sp,i);
        PRINTF("object_set %d,%d\n",key_sp,value_sp);
    }
    sp=sp_bak;
}

DEF_AST_METHOD(ObjectLiteral,AST_CODEGEN)
{
    PRINTF("new_object %d\n",sp);
    int sp_bak=sp++,this_bak=++this_count;
    int value_sp=sp,key_sp=sp+1;
    for(auto &x:nodes)
    {
        auto property=(AST_NAME(PropertyNameAndValue)*)x.get();
        CODEGEN(property->assignExpr);
        if(this_bak!=this_count)
        {
            PRINTF("set_this %d\n",sp_bak);
            this_bak=this_count;
        }
        PRINTF("load %d,%s\n",key_sp,GET_LITERAL(property->identifier).raw.c_str());
        PRINTF("object_set %d,%d\n",key_sp,value_sp);
    }
    sp=sp_bak;
}

