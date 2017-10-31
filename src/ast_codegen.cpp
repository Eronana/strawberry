#include "ast.h"
#include "ast_method.h"
#include <cstdlib>
int print_count=0;
int last_object_get=0;
#define PRINTF(...) print_count++,fprintf(AST::fp,__VA_ARGS__)

#define CODEGEN(M) M->codeGen(symbol)
#define AST_CODEGEN_ARRAY() {FOREACH(nodes){CODEGEN(x);}}


void variantMethod(Symbol *symbol,const string &id,const char *method)
{
    int allowIndex;
    switch(symbol->lookup(id,allowIndex))
    {
        case ST_GLOBAL:
            PRINTF("%s_global %d\n",method,allowIndex);
            break;
        case ST_LOCAL:
            PRINTF("%s %d\n",method,allowIndex);
            break;
        case ST_EXTERNAL:
        {
            auto &func=*functionStack.top();
            if(!func.ext_map.count(id))
            {
                func.ext_map[id]=func.ext_list.size();
                func.ext_list.push_back(id);
            }
            PRINTF("%s_external %d\n",method,func.ext_map[id]);
            break;
        }
        default:
            break;
    }
}
template<typename T>
void storeVariant(Symbol *symbol,T &identifier)
{
    variantMethod(symbol,GET_LITERAL(identifier).raw,"store");
}
template<typename T>
void loadVariant(Symbol *symbol,T &identifier)
{
    variantMethod(symbol,GET_LITERAL(identifier).raw,"load");
}
bool is_identifier(const unique_ptr<AST> &expr)
{
    AST_NAME(Literal)* ast=dynamic_cast<AST_NAME(Literal)*>(expr.get());
    if(ast==nullptr)return false;
    return ast->token.type==TOKEN_IDENTIFIER;
}

DEF_AST_METHOD(Literal,AST_CODEGEN)
{
    switch(token.type)
    {
        case TOKEN_IDENTIFIER:
        {
            AST_PTR tmp(this);
            loadVariant(symbol,tmp);
            tmp.release();
        }
        break;
        case TOKEN_BIN_INTEGER_LITERAL:
            PRINTF("push %ld\n",strtol(token.raw.c_str(),NULL,2));
            break;
        case TOKEN_OCT_INTEGER_LITERAL:
        case TOKEN_DEC_INTEGER_LITERAL:
        case TOKEN_HEX_INTEGER_LITERAL:
            PRINTF("push %ld\n",strtol(token.raw.c_str(),NULL,0));
            break;
        default:
            PRINTF("push %s\n",token.raw.c_str());
    }
}
DEF_AST_METHOD(ArrayLiteral,AST_CODEGEN)
{
    PRINTF("new_array\n");
    int this_bak=++this_count;
    FOREACH(nodes)
    {
        CODEGEN(x);
        if(this_bak!=this_count)
        {
            PRINTF("reset_this\n");
            this_bak=++this_count;
        }
        PRINTF("array_push\n");
    }
}
DEF_AST_METHOD(ObjectLiteral,AST_CODEGEN)
{
    PRINTF("new_object\n");
    int this_bak=++this_count;
    for(auto &x:nodes)
    {
        auto property=(AST_NAME(PropertyNameAndValue)*)x.get();
        CODEGEN(property->assignExpr);
        if(this_bak!=this_count)
        {
            PRINTF("reset_this\n");
            this_bak=++this_count;
        }
        if(is_identifier(property->identifier))PRINTF("push \"%s\"\n",GET_LITERAL(property->identifier).raw.c_str());
        else PRINTF("push %s\n",GET_LITERAL(property->identifier).raw.c_str());
        PRINTF("object_set\n");
        PRINTF("pop\n");
    }
}
DEF_AST_METHOD(Expression,AST_CODEGEN)
{
    if(nodes.size())CODEGEN(nodes[0]);
    FOR2ND(nodes)
    {
        PRINTF("pop\n");
        CODEGEN(nodes[i]);
    }
}
DEF_AST_METHOD(ArgumentList,AST_CODEGEN)
{
    bool isGlobalThis=last_object_get!=print_count;
    PRINTF("; callee\n");
    PRINTF("dup\n"); 
    PRINTF("; this\n");
    if(isGlobalThis)PRINTF("load_global 0\n");
    else PRINTF("get_this\n");
    for(int i=nodes.size()-1;~i;i--)
    {
        PRINTF("; arugment %d\n",i);
        CODEGEN(nodes[i]);
    }
    PRINTF("call %lu\n",nodes.size()+2);
}


DEF_AST_METHOD(CaseClauseList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(FormalParameterList,AST_CODEGEN) {}
DEF_AST_METHOD(StatementList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(OperationList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(VariableDeclarationList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(MemberExpressionPartList,AST_CODEGEN) AST_CODEGEN_ARRAY()
DEF_AST_METHOD(CallExpressionPartList,AST_CODEGEN) AST_CODEGEN_ARRAY()



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
    PRINTF("dup\n");
    PRINTF("set_this\n");
    int this_bak=++this_count;
    CODEGEN(expr);
    PRINTF("swap\n");
    if(this_bak!=this_count)
    {
        PRINTF("set_this\n");
        this_count++;
    
    }else PRINTF("pop\n");
    PRINTF("object_get\n");
    last_object_get=print_count;
}

DEF_AST_METHOD(MemberPartIdentifer,AST_CODEGEN)
{
    PRINTF("set_this\n");
    this_count++;
    PRINTF("push \"%s\"\n",GET_LITERAL(identifer).raw.c_str());
    PRINTF("object_get\n");
    last_object_get=print_count;
}

DEF_AST_METHOD(MemberExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    CODEGEN(exprPart);
}

DEF_AST_METHOD(CallExpression,AST_CODEGEN)
{
    CODEGEN(expr);
    CODEGEN(arguments);
    if(exprPart)CODEGEN(exprPart);
}

DEF_AST_METHOD(PostfixExpression,AST_CODEGEN)
{
    bool isIdentifier=is_identifier(expr);
    if(isIdentifier)loadVariant(symbol,expr);
    else CODEGEN(expr);
    PRINTF("dup\n");
    if(GET_LITERAL(optr).type==TOKEN_INCREMENT)PRINTF("inc\n");
    else PRINTF("dec\n");
    if(isIdentifier)storeVariant(symbol,expr);
    else PRINTF("object_reset\n");
    PRINTF("pop\n");
}


DEF_AST_METHOD(PrefixExpression,AST_CODEGEN)
{
    bool isIdentifier=is_identifier(expr);
    if(isIdentifier)loadVariant(symbol,expr);
    else CODEGEN(expr);

    if(GET_LITERAL(optr).type==TOKEN_INCREMENT||GET_LITERAL(optr).type==TOKEN_DECREMENT)
    {
        if(GET_LITERAL(optr).type==TOKEN_INCREMENT)PRINTF("inc\n");
        else PRINTF("dec\n");
        if(isIdentifier)storeVariant(symbol,expr);
        else PRINTF("object_reset\n");
        return;
    }

    switch(GET_LITERAL(optr).type)
    {
        case TOKEN_ADD:
            PRINTF("pos\n");
            break;
        case TOKEN_SUB:
            PRINTF("neg\n");
            break;
        case TOKEN_BITWISE_NOT:
            PRINTF("bnot\n");
            break;
        case TOKEN_NEGATION:
            PRINTF("lnot\n");
            break;
        default:
            PRINTF("typeof\n");
            break;
    }
}

DEF_AST_METHOD(BinaryOperationExpression,AST_CODEGEN)
{
    auto optype=GET_LITERAL(optr).type;
    int label;
    if(optype==TOKEN_LOGIC_AND)
    {
        label=nextLabel();
        PRINTF("dup\n");
        PRINTF("istrue\n");
        PRINTF("jmp label_%d\n",label);
        PRINTF("pop\n");
    }
    else if(optype==TOKEN_LOGIC_OR)
    {
        label=nextLabel();
        PRINTF("dup\n");
        PRINTF("isfalse\n");
        PRINTF("jmp label_%d\n",label);
        PRINTF("pop\n");
    }
    bool isIdentifier=is_identifier(expr);
    if(isIdentifier)loadVariant(symbol,expr);
    else CODEGEN(expr);
    switch(optype)
    {
        case TOKEN_MUL:
            PRINTF("mul\n");
            break;
        case TOKEN_DIV:
            PRINTF("div\n");
            break;
        case TOKEN_MOD:
            PRINTF("mod\n");
            break;
        case TOKEN_ADD:
            PRINTF("add\n");
            break;
        case TOKEN_SUB:
            PRINTF("sub\n");
            break;
        case TOKEN_BITWISE_AND:
            PRINTF("band\n");
            break;
        case TOKEN_BITWISE_XOR:
            PRINTF("xor\n");
            break;
        case TOKEN_BITWISE_OR:
            PRINTF("bor\n");
            break;
        case TOKEN_LEFT_SHIFT:
            PRINTF("shl\n");
            break;
        case TOKEN_RIGHT_SHIFT:
            PRINTF("shr\n");
            break;
        case TOKEN_LESS:
            PRINTF("less\n");
            break;
        case TOKEN_GREATER:
            PRINTF("gt\n");
            break;
        case TOKEN_LESS_EQUAL:
            PRINTF("le\n");
            break;
        case TOKEN_GREATER_EQUAL:
            PRINTF("ge\n");
            break;
        case TOKEN_EQUAL:
            PRINTF("eq\n");
            break;
        case TOKEN_INEQUAL:
            PRINTF("ieq\n");
            break;
        case TOKEN_LOGIC_AND:
        case TOKEN_LOGIC_OR:
            PRINTF("label_%d:\n",label);
            break;
        default:
            break;
    }
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
    PRINTF("istrue\n");
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
    bool isIdentifier=is_identifier(expr);
    if(isIdentifier&&GET_LITERAL(assignOptr).type==TOKEN_ASSIGN)
    {
        CODEGEN(assignExpr);
        storeVariant(symbol,expr);
        return;
    }
    CODEGEN(assignExpr);
    CODEGEN(expr);
    if(GET_LITERAL(assignOptr).type!=TOKEN_ASSIGN)PRINTF("swap\n");
    switch(GET_LITERAL(assignOptr).type)
    {
        case TOKEN_ASSIGN:
            PRINTF("pop\n");
            break;
        case TOKEN_MUL_ASSIGN:
            PRINTF("mul\n");
            break;
        case TOKEN_DIV_ASSIGN:
            PRINTF("div\n");
            break;
        case TOKEN_MOD_ASSIGN:
            PRINTF("mod\n");
            break;
        case TOKEN_ADD_ASSIGN:
            PRINTF("add\n");
            break;
        case TOKEN_SUB_ASSIGN:
            PRINTF("sub\n");
            break;
        case TOKEN_LEFT_SHIFT_ASSIGN:
            PRINTF("shl\n");
            break;
        case TOKEN_RIGHT_SHIFT_ASSIGN:
            PRINTF("shr\n");
            break;
        case TOKEN_BITWISE_AND_ASSIGN:
            PRINTF("and\n");
            break;
        case TOKEN_BITWISE_XOR_ASSIGN:
            PRINTF("xor\n");
            break;
        case TOKEN_BITWISE_OR_ASSIGN:
            PRINTF("or\n");
            break;
        default:
            break;
    }
    if(isIdentifier)storeVariant(symbol,expr);
    else PRINTF("object_reset\n");
}

DEF_AST_METHOD(ExpressionStatement,AST_CODEGEN)
{
    CODEGEN(expr);
    PRINTF("pop\n");
}

DEF_AST_METHOD(IfStatement,AST_CODEGEN)
{
    PRINTF("; if statement\n");
    CODEGEN(expr);
    PRINTF("istrue\n");
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
    storeVariant(symbol,identifier);
    PRINTF("pop\n");
}

DEF_AST_METHOD(ForStatement,AST_CODEGEN)
{
    int beginLabel=nextLabel();
    int endLabel=nextLabel();
    int continueLabel=nextLabel();
    continueStack.push(continueLabel);
    breakStack.push(endLabel);
    GET_SCOPE();
    PRINTF("; for statement\n");
    PRINTF("; init\n");
    CODEGEN(init);
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("istrue\n");
    PRINTF("jmp label_%d\n",endLabel);
    PRINTF("; statement\n");
    CODEGEN(statement);
    PRINTF("label_%d:\n",continueLabel);
    PRINTF("; loop expression\n");
    CODEGEN(loopExpr);
    PRINTF("pop\n");
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
    GET_SCOPE();
    PRINTF("; do statement\n");
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; statement\n");
    CODEGEN(statement);
    PRINTF("label_%d:\n",continueLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("isfalse\n");
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
    GET_SCOPE();
    PRINTF("; while statement\n");
    PRINTF("label_%d:\n",beginLabel);
    PRINTF("; check expression\n");
    CODEGEN(expr);
    PRINTF("istrue\n");
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
    if(expr)
    {
        PRINTF("dup\n");
        CODEGEN(expr);
        PRINTF("eq\n");
        PRINTF("istrue\n");
        PRINTF("jmp label_%d\n",nextCaluse);
        PRINTF("label_%d:\n",nextLabel());
        CODEGEN(stmtList);
    }
    else
    {
        int defaultLabel=nextLabel();
        int lastLabel=breakStack.top();
        defaultClauseStack.top()=defaultLabel;
        if(lastLabel+1==nextCaluse)PRINTF("jmp label_%d\n",nextCaluse);
        PRINTF("; default clause\n");
        PRINTF("label_%d:\n",defaultLabel);
        CODEGEN(stmtList);
    }
    PRINTF("jmp label_%d\n",nextCaluse+3);
    PRINTF("label_%d:\n",nextCaluse);
}
DEF_AST_METHOD(CaseBlock,AST_CODEGEN)
{
    int endLabel=nextLabel();
    breakStack.push(endLabel);
    defaultClauseStack.push(-1);
    CODEGEN(caseClausesList);
    int defaultLabel=defaultClauseStack.top();
    if(defaultLabel!=-1)PRINTF("jmp label_%d\n",defaultLabel);
    PRINTF("label_%d:\n",endLabel);
    breakStack.pop();
    defaultClauseStack.pop();
}
DEF_AST_METHOD(SwitchStatement,AST_CODEGEN)
{
    GET_SCOPE();
    PRINTF("; switch statement\n");
    CODEGEN(expr);
    CODEGEN(caseBlock);
    PRINTF("pop\n");
}

vector<FILE *> func_tmpfile;
DEF_AST_METHOD(FunctionExpression,AST_CODEGEN)
{
    auto symbol_bak=symbol;
    GET_SCOPE();
    int funcLabel=nextLabel();
    FunctionInfo func;
    functionStack.push(&func);
    FILE *bak=AST::fp;
    FILE *tmp_fp=AST::fp=tmpfile();
    func_tmpfile.push_back(tmp_fp);
    PRINTF("; function expression\n");
    PRINTF("label_%d:\n",funcLabel);
    auto localCount=symbol->localCount;
    if(localCount)PRINTF("addsp %d\n",symbol->localCount);
    CODEGEN(block);
    PRINTF("push null\n");
    PRINTF("ret\n");
    // if(localCount)PRINTF("subsp %d\n",symbol->localCount);
    functionStack.pop();
    AST::fp=bak;
    this_count++;
    PRINTF("new_array\n");
    for(auto &x:func.ext_list)
    {
        variantMethod(symbol_bak,x,"load");
        PRINTF("array_push\n");
    }
    PRINTF("create_function label_%d\n",funcLabel);
}

DEF_AST_METHOD(ReturnStatement,AST_CODEGEN)
{
    if(expr)CODEGEN(expr);
    else PRINTF("push null\n");
    PRINTF("ret\n");
}


DEF_AST_METHOD(ContinueStatement,AST_CODEGEN)
{
    PRINTF("jmp label_%d\n",continueStack.top());
}

DEF_AST_METHOD(BreakStatement,AST_CODEGEN)
{
    PRINTF("jmp label_%d\n",breakStack.top());
}

DEF_AST_METHOD(EmptyStatement,AST_CODEGEN){}

void copy_stream(FILE *dest,FILE *src)
{
    fseek(src,0,SEEK_SET);
    for(int c;(c=fgetc(src))!=EOF;fputc(c,dest));
    fclose(src);
}

DEF_AST_METHOD(Program,AST_CODEGEN)
{
    GET_SCOPE();
    auto localCount=symbol->localCount;
    if(localCount)PRINTF("addsp %d\n",symbol->localCount);
    CODEGEN(stmtList);
    if(localCount)PRINTF("subsp %d\n",symbol->localCount);
    PRINTF("halt\n\n");
    for(FILE *x:func_tmpfile)copy_stream(AST::fp,x);
}
