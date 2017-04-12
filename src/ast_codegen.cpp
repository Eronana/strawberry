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
ftest a
    if (FALSE(sp[a])) skip(next_instruction);

jmp a
    ip=a

swap a,b
    swap(reg[a],reg[b])

mul a,b
    reg[a]*=reg[b]

div a,b
    reg[a]/=reg[b]

mod a,b
    reg[a]%=reg[b]

add a,b
    reg[a]+=reg[b]

sub a,b
    reg[a]-=reg[b]

shl a,b
    reg[a]<<=reg[b]

shr a,b
    reg[a]>>=reg[b]

and a,b
    reg[a]&=reg[b]

xor a,b
    reg[a]^=reg[b]

or a,b
    reg[a]|=reg[b]

set_rv a
    return_value=reg[a]

ret
    return
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
        CODEGEN(nodes[i]);
    }
    PRINTF("call %d,%lu\n",sp,nodes.size());
    sp-=nodes.size();
}

DEF_AST_METHOD(CallExpressionPartList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(OperationList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(CaseClauseList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(FormalParameterList,AST_CODEGEN) {}
DEF_AST_METHOD(StatementList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(VariableDeclarationList,AST_CODEGEN) AST_CODEGEN_ARRAY()

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
        PRINTF("set_this %d\n",sp);
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

void variantMethod(Symbol *symbol,AST_PTR &identifier,int reg,const char *method)
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

DEF_AST_METHOD(PostfixExpression,AST_CODEGEN)
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


DEF_AST_METHOD(PrefixExpression,AST_CODEGEN)
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

DEF_AST_METHOD(BinaryExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    CODEGEN(operations);
}

DEF_AST_METHOD(ConditionalExpression,AST_CODEGEN)
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

DEF_AST_METHOD(AssignmentExpression,AST_CODEGEN)
{
    bool isIdentifier=dynamic_cast<AST_NAME(Literal)*>(expr.get())!=nullptr;
    if(isIdentifier&&GET_LITERAL(assignOptr).type==TOKEN_ASSIGN)
    {
        CODEGEN(assignExpr);
        setVariant(symbol,expr,sp);
        return;
    }
    CODEGEN(assignExpr);
    sp++;
    CODEGEN(expr);
    sp--;
    int a_sp=sp,b_sp=sp+1;
    PRINTF("swap %d,%d\n",a_sp,b_sp);
    switch(GET_LITERAL(assignOptr).type)
    {
        case TOKEN_ASSIGN:
            PRINTF("mov %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_MUL_ASSIGN:
            PRINTF("mul %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_DIV_ASSIGN:
            PRINTF("div %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_MOD_ASSIGN:
            PRINTF("mod %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_ADD_ASSIGN:
            PRINTF("add %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_SUB_ASSIGN:
            PRINTF("sub %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_LEFT_SHIFT_ASSIGN:
            PRINTF("shl %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_RIGHT_SHIFT_ASSIGN:
            PRINTF("shr %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_BITWISE_AND_ASSIGN:
            PRINTF("and %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_BITWISE_XOR_ASSIGN:
            PRINTF("xor %d,%d\n",a_sp,b_sp);
            break;
        case TOKEN_BITWISE_OR_ASSIGN:
            PRINTF("or %d,%d\n",a_sp,b_sp);
            break;
    }
    if(isIdentifier)setVariant(symbol,expr,sp);
    else PRINTF("object_reset %d\n",sp);
}

DEF_AST_METHOD(ExpressionStatement,AST_CODEGEN)
{
    CODEGEN(expr);
}

DEF_AST_METHOD(IfStatement,AST_CODEGEN)
{
    PRINTF("; if statement\n");
    CODEGEN(expr);
    PRINTF("test %d\n",sp);
    int jmpLabel=nextLabel();
    int elseLabel;
    if(elseStatement)elseLabel=nextLabel();
    else elseLabel=jmpLabel;
    PRINTF("jmp label_%d\n",elseLabel);
    PRINTF("; true branch\n");
    CODEGEN(statement);
    if(elseStatement)
    {
        PRINTF("jmp label_%d\n",jmpLabel);
        PRINTF("label_%d:\n",elseLabel);
        PRINTF("; else branch\n");
        CODEGEN(elseStatement);
    }
    PRINTF("label_%d:\n",jmpLabel);
}

DEF_AST_METHOD(VariableDeclaration,AST_CODEGEN)
{
    if(!assignExpr)return;
    CODEGEN(assignExpr);
    setVariant(symbol,identifier,sp);
}

DEF_AST_METHOD(ForStatement,AST_CODEGEN)
{
    int beginLabel=nextLabel();
    int endLabel=nextLabel();
    int continueLabel=nextLabel();
    continueStack.push(continueLabel);
    breakStack.push(endLabel);

    PRINTF("; for statement\n");
    PRINTF("; init\n");
    CODEGEN(init);
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("test %d\n",sp);
    PRINTF("jmp label_%d\n",endLabel);
    PRINTF("; statement\n");
    CODEGEN(statement);
    PRINTF("label_%d:\n",continueLabel);
    PRINTF("; loop expression\n");
    CODEGEN(loopExpr);
    PRINTF("jmp label_%d\n",beginLabel);
    PRINTF("label_%d:\n",endLabel);
    continueStack.pop();
    breakStack.pop();
}

DEF_AST_METHOD(DoStatement,AST_CODEGEN)
{
    int beginLabel=nextLabel();
    int endLabel=nextLabel();
    int continueLabel=nextLabel();
    continueStack.push(continueLabel);
    breakStack.push(endLabel);

    PRINTF("; do statement\n");
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; statement\n");
    CODEGEN(statement);
    PRINTF("label_%d:\n",continueLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("ftest %d\n",sp);
    PRINTF("jmp label_%d\n",beginLabel);
    PRINTF("label_%d:\n",endLabel);
    continueStack.pop();
    breakStack.pop();
}

DEF_AST_METHOD(WhileStatement,AST_CODEGEN)
{
    int beginLabel=nextLabel();
    int endLabel=nextLabel();
    continueStack.push(beginLabel);
    breakStack.push(endLabel);

    PRINTF("; while statement\n");
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("test %d\n",sp);
    PRINTF("jmp label_%d\n",endLabel);
    PRINTF("; statement\n");
    CODEGEN(statement);
    PRINTF("jmp label_%d\n",beginLabel);
    PRINTF("label_%d:\n",endLabel);

    continueStack.pop();
    breakStack.pop();
}

DEF_AST_METHOD(CaseClause,AST_CODEGEN)
{
    int nextCaluse=nextLabel();
    CODEGEN(expr);
    PRINTF("eq %d %d\n",sp,sp-1);
    PRINTF("jmp label_%d\n",nextCaluse);
    CODEGEN(stmtList);
    PRINTF("label_%d:\n",nextCaluse);
}
DEF_AST_METHOD(CaseBlock,AST_CODEGEN)
{
    int endLabel=nextLabel();
    breakStack.push(endLabel);
    CODEGEN(caseClausesList);
    PRINTF("label_%d:\n",endLabel);
    breakStack.pop();
}
DEF_AST_METHOD(SwitchStatement,AST_CODEGEN)
{
    PRINTF("; switch statement\n");
    CODEGEN(expr);
    sp++;
    CODEGEN(caseBlock);
    sp--;
}
DEF_AST_METHOD(FunctionExpression,AST_CODEGEN)
{
    PRINTF("[FUNCTION]\n");
}

DEF_AST_METHOD(ReturnStatement,AST_CODEGEN)
{
    if(expr)CODEGEN(expr);
    else PRINTF("load %d,null\n",sp);
    PRINTF("set_rv %d\n",sp);
    PRINTF("ret\n");
}


DEF_AST_METHOD(ContinueStatement,AST_CODEGEN)
{
    printf("jmp label_%d\n",continueStack.top());
}

DEF_AST_METHOD(BreakStatement,AST_CODEGEN)
{
    printf("jmp label_%d\n",breakStack.top());
}

DEF_AST_METHOD(EmptyStatement,AST_CODEGEN){}

DEF_AST_METHOD(Program,AST_CODEGEN)
{
    CODEGEN(stmtList);
}
