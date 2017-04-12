#include "ast.h"

#define CODEGEN(M) M->codeGen(symbol)
#define AST_CODEGEN_ARRAY() {FOREACH(nodes){CODEGEN(x);}}

/*
new_array a
    reg[a]=new Array
    this=reg[a]

new_object a
    reg[a]=new Object
    this=reg[a]

set_this a
    this=reg[a]

load a,b
    reg[a]=b

object_set a,b
    this[a]=b

call sp,argc
    call function

object_get a,b
    this[a]=this[b]

inc a
    reg[a]++

dec a
    reg[a]--

object_reset a
    this[last_key]=reg[a]

test a
    if (TRUE(sp[a])) skip(next_instruction);

jmp a
    ip=a
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
            this_bak=++this_count;
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
            this_bak=++this_count;
        }
        PRINTF("load %d,%s\n",key_sp,GET_LITERAL(property->identifier).raw.c_str());
        PRINTF("object_set %d,%d\n",key_sp,value_sp);
    }
    sp=sp_bak;
}

DEF_AST_METHOD(ArgumentList,AST_CODEGEN)
{
    for(int i=nodes.size()-1;~i;i--,sp++)
    {
        PRINTF("; arugment %d\n",i);
        CODEGEN(vec[i]);
    }
    PRINTF("call %d,%u\n",sp,nodes.size());
    sp-=nodes.size();
}

DEF_AST_METHOD(CallExpressionPartList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(OperationList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(CaseClauseList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(FormalParameterList,AST_CODEGEN) {}
DEF_AST_METHOD(StatementList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(VariableDeclarationList) AST_CODEGEN_ARRAY()

DEF_AST_METHOD(Block,AST_CODEGEN)
{
    GET_SCOPE();
    AST_CODEGEN_ARRAY()
}

DEF_AST_METHOD(PrimaryExpression,AST_CODEGEN)
{
    CODEGEN(expr);
}
DEF_AST_METHOD(PropertyNameAndValue,AST_CODEGEN) {}


DEF_AST_METHOD(MemberPartExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    PRINTF("object_get %d,%d\n",sp,sp);
}

DEF_AST_METHOD(MemberPartIdentifer,AST_CODEGEN)
{
    PRINTF("load %d,%s",sp,GET_LITERAL(identifer).raw.c_str());
    PRINTF("object_get %d,%d\n",sp,sp);
}

DEF_AST_METHOD(MemberExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    CODEGEN(exprPart);
}

DEF_AST_METHOD(MemberExpressionPartList,AST_CODEGEN)
{
    FOREACH(nodes)
    {
        PRINTF("set_this %s\n",sp);
        this_count++;
        CODEGEN(x);
    }
}

DEF_AST_METHOD(CallExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    sp++;
    CODEGEN(arguments);
    sp--;
    if(exprPart)CODEGEN(exprPart);
}

void variantMethod(Symbol *symbol,AST_PTR &identifier,int reg,char *method)
{
    string &id=GET_LITERAL(identifier).raw;
    int allowIndex=symbol->lookup(id);
    if(Symbol::isGlobal(allowIndex))PRINTF("%s_global %d,%d\n",method,reg,Symbol::getGlobalIndex(allowIndex));
    else
    {
        if(method[0]=='g')PRINTF("mov %d,%d\n",reg,allowIndex);
        else PRINTF("mov %d,%d\n",allowIndex,reg);
    }
}
void setVariant(Symbol *symbol,AST_PTR &identifier,int reg)
{
    variantMethod(symbol,identifier,reg,"set");
}

void getVariant(Symbol *symbol,AST_PTR &identifier,int reg)
{
    variantMethod(symbol,identifier,reg,"get");
}

AST_CODEGEN(PostfixExpression)
{
    bool isIdentifier=dynamic_cast<AST_NAME(Literal)*>(expr.get())!=nullptr;
    if(isIdentifier)getVariant(symbol,expr,sp);
    else CODEGEN(expr);
    PRINTF("mov %d,%d\n",sp+1,sp);
    if(GET_LITERAL(optr).type==TOKEN_INCREMENT)PRINTF("inc");
    else PRINTF("dec");
    PRINTF(" %d\n",sp+1);
    if(isIdentifier)setVariant(symbol,expr,sp+1);
    else PRINTF("object_reset %d\n",sp+1);
}


AST_CODEGEN(PrefixExpression)
{
    bool isIdentifier=dynamic_cast<AST_NAME(Literal)*>(expr.get())!=nullptr;
    if(isIdentifier)getVariant(symbol,expr,sp);
    else CODEGEN(expr);
    if(GET_LITERAL(optr).type==TOKEN_INCREMENT)PRINTF("inc");
    else PRINTF("dec");
    PRINTF(" %d\n",sp);
    if(isIdentifier)setVariant(symbol,expr,sp);
    else PRINTF("object_reset %d\n",sp);
}

AST_CODEGEN(BinaryExpression)
{
    CODEGEN(expr);
    CODEGEN(operations);
}

AST_CODEGEN(ConditionalExpression)
{
    PRINTF("; conditional expression\n");
    CODEGEN(expr);
    PRINTF("test %d\n",sp);
    int falseLabel=nextLabel();
    int jmpLabel=nextLabel();
    PRINTF("jmp label_%d\n",falseLabel);
    PRINTF("; true branch\n");
    CODEGEN(trueExpr);
    PRINTF("jmp label_%d\n",jmpLabel);
    PRINTF("label_%d:\n",falseLabel);
    PRINTF("; false branch\n");
    CODEGEN(falseExpr);
    PRINTF("label_%d:\n",jmpLabel);
}
