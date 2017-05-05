#include "ast.h"
#include "exception.h"
#include "ast_method.h"

#define ADDSYMBOL(M) if(!symbol->insert(((AST_NAME(Literal)*)M.get())->token.raw))CHECK_ERROR(((AST_NAME(Literal)*)M.get())->token,"redefinition")
#define ADD_SCOPE() scope_map.insert(make_pair(this,unique_ptr<Symbol>(symbol)))
#define ENTRY_SCOPE(NAME) symbol=new Symbol(symbol,PP_JOIN(SCOPE_,NAME));ADD_SCOPE()
#define EXIT_SCOPE() symbol->setParentCount(),symbol=symbol->getParent()
#define FILLAST(M) M->fill(symbol)
#define AST_FILL_ARRAY() {FOREACH(nodes){FILLAST(x);}}

DEF_AST_METHOD(Literal,AST_FILL) {}

DEF_AST_METHOD(ArrayLiteral,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(ObjectLiteral,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(Expression,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(MemberExpressionPartList,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(ArgumentList,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(CallExpressionPartList,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(OperationList,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(CaseClauseList,AST_FILL) AST_FILL_ARRAY()
DEF_AST_METHOD(StatementList,AST_FILL) AST_FILL_ARRAY()


DEF_AST_METHOD(FormalParameterList,AST_FILL)
{
    for(int i=0;i<nodes.size();i++)
    {
        symbol->insert(((AST_NAME(Literal)*)nodes[i].get())->token.raw,~i);
    }
}

DEF_AST_METHOD(VariableDeclarationList,AST_FILL) AST_FILL_ARRAY()

DEF_AST_METHOD(Block,AST_FILL)
{
    ENTRY_SCOPE(BLOCK);
    AST_FILL_ARRAY()
    EXIT_SCOPE();
}


DEF_AST_METHOD(PrimaryExpression,AST_FILL)
{
    FILLAST(expr);
}

DEF_AST_METHOD(PropertyNameAndValue,AST_FILL)
{
    FILLAST(assignExpr);
}

DEF_AST_METHOD(MemberPartExpression,AST_FILL)
{
    FILLAST(expr);
}

DEF_AST_METHOD(MemberPartIdentifer,AST_FILL){}


DEF_AST_METHOD(MemberExpression,AST_FILL)
{
    FILLAST(expr);
    FILLAST(exprPart);
}

DEF_AST_METHOD(CallExpression,AST_FILL)
{
    FILLAST(expr);
    FILLAST(arguments);
    if(exprPart)FILLAST(exprPart);
}

DEF_AST_METHOD(PostfixExpression,AST_FILL)
{
    FILLAST(expr);
}

DEF_AST_METHOD(PrefixExpression,AST_FILL)
{
    FILLAST(expr);
}

DEF_AST_METHOD(BinaryOperationExpression,AST_FILL)
{
    FILLAST(expr);
}

DEF_AST_METHOD(BinaryExpression,AST_FILL)
{
    FILLAST(expr);
    FILLAST(operations);
}

DEF_AST_METHOD(ConditionalExpression,AST_FILL)
{
    FILLAST(expr);
    FILLAST(trueExpr);
    FILLAST(falseExpr);
}

DEF_AST_METHOD(AssignmentExpression,AST_FILL)
{
    FILLAST(expr);
    FILLAST(assignExpr);
}

DEF_AST_METHOD(ExpressionStatement,AST_FILL)
{
    if(expr)FILLAST(expr);
}

DEF_AST_METHOD(IfStatement,AST_FILL)
{
    FILLAST(expr);
    FILLAST(statement);
    if(elseStatement)FILLAST(elseStatement);
}

DEF_AST_METHOD(VariableDeclaration,AST_FILL)
{
    ADDSYMBOL(identifier);
    if(assignExpr)FILLAST(assignExpr);
}

DEF_AST_METHOD(ForStatement,AST_FILL)
{
    ENTRY_SCOPE(FOR);
    FILLAST(init);
    FILLAST(expr);
    FILLAST(loopExpr);
    FILLAST(statement);
    EXIT_SCOPE();
}

DEF_AST_METHOD(DoStatement,AST_FILL)
{
    ENTRY_SCOPE(DO);
    FILLAST(statement);
    FILLAST(expr);
    EXIT_SCOPE();
}

DEF_AST_METHOD(WhileStatement,AST_FILL)
{
    ENTRY_SCOPE(WHILE);
    FILLAST(expr);
    FILLAST(statement);
    EXIT_SCOPE();
}

DEF_AST_METHOD(CaseClause,AST_FILL)
{
    if(expr)FILLAST(expr);
    FILLAST(stmtList);
}

DEF_AST_METHOD(CaseBlock,AST_FILL)
{
    FILLAST(caseClausesList);
}

DEF_AST_METHOD(SwitchStatement,AST_FILL)
{
    ENTRY_SCOPE(SWITCH);
    FILLAST(expr);
    FILLAST(caseBlock);
    EXIT_SCOPE();
}

DEF_AST_METHOD(FunctionExpression,AST_FILL)
{
    ENTRY_SCOPE(FUNCTION);
    if(formalParameterList)FILLAST(formalParameterList);
    FILLAST(block);
    EXIT_SCOPE();
}

DEF_AST_METHOD(ReturnStatement,AST_FILL)
{
    if(expr)FILLAST(expr);
}

DEF_AST_METHOD(ContinueStatement,AST_FILL){}


DEF_AST_METHOD(BreakStatement,AST_FILL){}


DEF_AST_METHOD(EmptyStatement,AST_FILL){}


DEF_AST_METHOD(Program,AST_FILL)
{
    ENTRY_SCOPE(GLOBAL);
    symbol->insert("system");
    FILLAST(stmtList);
    EXIT_SCOPE();
}
