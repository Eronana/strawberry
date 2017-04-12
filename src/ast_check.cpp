#include "ast.h"
#include "narg.h"
#include "exception.h"
#include "lexer.h"

#define CHECKAST(M) M->check(symbol)
#define AST_CHECK_ARRAY() {FOREACH(nodes){CHECKAST(x);}}
#define MATCHSCOPE(M) symbol->matchScope(PP_JOIN(SCOPE_,M))


DEF_AST_METHOD(Literal,AST_CHECK)
{
    if(token.type==TOKEN_IDENTIFIER&&Symbol::isEmpty(symbol->lookup(token.raw)))
        CHECK_ERROR(token,"use of undeclared identifier");
}

DEF_AST_METHOD(ArrayLiteral,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(ObjectLiteral,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(Expression,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(MemberExpressionPartList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(ArgumentList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(CallExpressionPartList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(OperationList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(CaseClauseList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(FormalParameterList,AST_CHECK) {}
DEF_AST_METHOD(StatementList,AST_CHECK) AST_CHECK_ARRAY()
DEF_AST_METHOD(VariableDeclarationList,AST_CHECK) AST_CHECK_ARRAY()

DEF_AST_METHOD(Block,AST_CHECK)
{
    GET_SCOPE();
    AST_CHECK_ARRAY()
}

DEF_AST_METHOD(PrimaryExpression,AST_CHECK)
{
    CHECKAST(expr);
}

DEF_AST_METHOD(PropertyNameAndValue,AST_CHECK)
{
    CHECKAST(assignExpr);
}

DEF_AST_METHOD(MemberPartExpression,AST_CHECK)
{
    CHECKAST(expr);
}

DEF_AST_METHOD(MemberPartIdentifer,AST_CHECK){}

DEF_AST_METHOD(MemberExpression,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(exprPart);
}

DEF_AST_METHOD(CallExpression,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(arguments);
    if(exprPart)CHECKAST(exprPart);
}

DEF_AST_METHOD(PostfixExpression,AST_CHECK)
{
    CHECKAST(expr);
    if(dynamic_cast<AST_NAME(Literal)*>(expr.get())&&GET_LITERAL(expr).type!=TOKEN_IDENTIFIER)CHECK_ERROR(GET_LITERAL(expr),"unexpection literal");
}

DEF_AST_METHOD(PrefixExpression,AST_CHECK)
{
    CHECKAST(expr);
}

DEF_AST_METHOD(BinaryExpression,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(operations);
}

DEF_AST_METHOD(ConditionalExpression,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(trueExpr);
    CHECKAST(falseExpr);
}

DEF_AST_METHOD(AssignmentExpression,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(assignExpr);
}

DEF_AST_METHOD(ExpressionStatement,AST_CHECK)
{
    if(expr)CHECKAST(expr);
}

DEF_AST_METHOD(IfStatement,AST_CHECK)
{
    CHECKAST(expr);
    CHECKAST(statement);
    if(elseStatement)CHECKAST(elseStatement);
}

DEF_AST_METHOD(VariableDeclaration,AST_CHECK)
{
    CHECKAST(identifier);
    if(assignExpr)CHECKAST(assignExpr);
}

DEF_AST_METHOD(ForStatement,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(init);
    CHECKAST(expr);
    CHECKAST(loopExpr);
    CHECKAST(statement);
}

DEF_AST_METHOD(DoStatement,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(statement);
    CHECKAST(expr);
}

DEF_AST_METHOD(WhileStatement,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(expr);
    CHECKAST(statement);
}

DEF_AST_METHOD(CaseClause,AST_CHECK)
{
    if(expr)CHECKAST(expr);
    CHECKAST(stmtList);
}

DEF_AST_METHOD(CaseBlock,AST_CHECK)
{
    CHECKAST(caseClausesList);
}

DEF_AST_METHOD(SwitchStatement,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(expr);
    CHECKAST(caseBlock);
}

DEF_AST_METHOD(FunctionExpression,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(block);
}

DEF_AST_METHOD(ReturnStatement,AST_CHECK)
{
    if(!MATCHSCOPE(FUNCTION))CHECK_ERROR(GET_LITERAL(literal),"unexpection return");
    if(expr)CHECKAST(expr);
}

DEF_AST_METHOD(ContinueStatement,AST_CHECK)
{
    if(
        !MATCHSCOPE(FOR)&&
        !MATCHSCOPE(DO)&&
        !MATCHSCOPE(WHILE))CHECK_ERROR(GET_LITERAL(literal),"unexpection continue");
}

DEF_AST_METHOD(BreakStatement,AST_CHECK)
{
    if(
        !MATCHSCOPE(FOR)&&
        !MATCHSCOPE(DO)&&
        !MATCHSCOPE(WHILE)&&
        !MATCHSCOPE(SWITCH))CHECK_ERROR(GET_LITERAL(literal),"unexpection break");
}

DEF_AST_METHOD(EmptyStatement,AST_CHECK){}

DEF_AST_METHOD(Program,AST_CHECK)
{
    GET_SCOPE();
    CHECKAST(stmtList);
}

