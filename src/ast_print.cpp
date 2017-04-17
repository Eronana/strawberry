#include "ast.h"
#include "ast_method.h"

#define PRINTAST(M) M->print()
#define PRINT_NODES() FOREACH(nodes)PRINTAST(x)
#define AST_PRINT_ARRAY() {PRINT_NODES();}

#define AST_PRINT_BKT_ARRAY(...) { PP_JOIN(AST_PRINT_BKT_ARRAY_,PP_NARG(__VA_ARGS__))(__VA_ARGS__) }
#define AST_PRINT_BKT_ARRAY_0() \
if(!nodes.empty())\
{\
    PRINTAST(nodes[0]);\
    FOR2ND(nodes)\
    {\
        PUTS(",");\
        PRINTAST(nodes[i]);\
    }\
}

#define AST_PRINT_BKT_ARRAY_2(LBARCKET,RBARCKET) \
PUTS(LBARCKET);\
AST_PRINT_BKT_ARRAY_0()\
PUTS(RBARCKET);


DEF_AST_METHOD(Literal,AST_PRINT)
{
    fputs(token.raw.c_str(),fp);
}

DEF_AST_METHOD(ArrayLiteral,AST_PRINT) AST_PRINT_BKT_ARRAY("[","]")
DEF_AST_METHOD(ObjectLiteral,AST_PRINT) AST_PRINT_BKT_ARRAY("{","}")
DEF_AST_METHOD(Expression,AST_PRINT) AST_PRINT_BKT_ARRAY()
DEF_AST_METHOD(MemberExpressionPartList,AST_PRINT) AST_PRINT_ARRAY()
DEF_AST_METHOD(ArgumentList,AST_PRINT) AST_PRINT_BKT_ARRAY("(",")")
DEF_AST_METHOD(CallExpressionPartList,AST_PRINT) AST_PRINT_ARRAY()
DEF_AST_METHOD(OperationList,AST_PRINT) AST_PRINT_ARRAY()
DEF_AST_METHOD(CaseClauseList,AST_PRINT) AST_PRINT_ARRAY()
DEF_AST_METHOD(FormalParameterList,AST_PRINT) AST_PRINT_BKT_ARRAY()
DEF_AST_METHOD(StatementList,AST_PRINT) AST_PRINT_ARRAY()

DEF_AST_METHOD(VariableDeclarationList,AST_PRINT)
{
    PUTS("var ");
    PRINT_NODES();
    PUTS(";\n");
}

DEF_AST_METHOD(Block,AST_PRINT)
{
    PUTS("{\n");
    PRINT_NODES();
    PUTS("}\n");
}

DEF_AST_METHOD(PrimaryExpression,AST_PRINT)
{
    PUTS("(");
    PRINTAST(expr);
    PUTS(")");
}

DEF_AST_METHOD(PropertyNameAndValue,AST_PRINT)
{
    PRINTAST(identifier);
    PUTS(": ");
    PRINTAST(assignExpr);
}

DEF_AST_METHOD(MemberPartExpression,AST_PRINT)
{
    PUTS("[");
    PRINTAST(expr);
    PUTS("]");
}

DEF_AST_METHOD(MemberPartIdentifer,AST_PRINT)
{
    PUTS(".");
    PRINTAST(identifer);
}

DEF_AST_METHOD(MemberExpression,AST_PRINT)
{
    PRINTAST(expr);
    PRINTAST(exprPart);
}

DEF_AST_METHOD(CallExpression,AST_PRINT)
{
    PRINTAST(expr);
    PRINTAST(arguments);
    if(exprPart)PRINTAST(exprPart);
}

DEF_AST_METHOD(PostfixExpression,AST_PRINT)
{
    PRINTAST(expr);
    PRINTAST(optr);
}

DEF_AST_METHOD(PrefixExpression,AST_PRINT)
{
    PRINTAST(optr);
    PRINTAST(expr);
}

DEF_AST_METHOD(BinaryOperationExpression,AST_PRINT)
{
    PRINTAST(optr);
    PRINTAST(expr);
}

DEF_AST_METHOD(BinaryExpression,AST_PRINT)
{
    PRINTAST(expr);
    PRINTAST(operations);
}

DEF_AST_METHOD(ConditionalExpression,AST_PRINT)
{
    PRINTAST(expr);
    PUTS(" ? ");
    PRINTAST(trueExpr);
    PUTS(" : ");
    PRINTAST(falseExpr);
}

DEF_AST_METHOD(AssignmentExpression,AST_PRINT)
{
    PRINTAST(expr);
    PRINTAST(assignOptr);
    PRINTAST(assignExpr);
}

DEF_AST_METHOD(ExpressionStatement,AST_PRINT)
{
    if(expr)PRINTAST(expr);
    PUTS(";\n");
}

DEF_AST_METHOD(IfStatement,AST_PRINT)
{
    PUTS("if (");
    PRINTAST(expr);
    PUTS(") ");
    PRINTAST(statement);
    if(elseStatement)
    {
        PUTS("else ");
        PRINTAST(elseStatement);
    }
}

DEF_AST_METHOD(VariableDeclaration,AST_PRINT)
{
    PRINTAST(identifier);
    if(assignExpr)
    {
        PUTS(" = ");
        PRINTAST(assignExpr);
    }
}

DEF_AST_METHOD(ForStatement,AST_PRINT)
{
    PUTS("for (");
    PRINTAST(init);
    PRINTAST(expr);
    PUTS(";");
    PRINTAST(loopExpr);
    PUTS(") ");
    PRINTAST(statement);
}

DEF_AST_METHOD(DoStatement,AST_PRINT)
{
    PUTS("do ");
    PRINTAST(statement);
    PUTS("while (");
    PRINTAST(expr);
    PUTS(");\n");
}

DEF_AST_METHOD(WhileStatement,AST_PRINT)
{
    PUTS("while (");
    PRINTAST(expr);
    PUTS(")");
    PRINTAST(statement);
}

DEF_AST_METHOD(CaseClause,AST_PRINT)
{
    if(expr)
    {
        PUTS("case ");
        PRINTAST(expr);
    }
    else PUTS("default\n");
    PUTS(":\n");
    PRINTAST(stmtList);
    PUTS("\n");
}

DEF_AST_METHOD(CaseBlock,AST_PRINT)
{
    PUTS("{\n");
    PRINTAST(caseClausesList);
    PUTS("}\n");
}

DEF_AST_METHOD(SwitchStatement,AST_PRINT)
{
    PUTS("switch (");
    PRINTAST(expr);
    PUTS(") ");
    PRINTAST(caseBlock);
}

DEF_AST_METHOD(FunctionExpression,AST_PRINT)
{
    PUTS("function(");
    if(formalParameterList)PRINTAST(formalParameterList);
    PUTS(")");
    PRINTAST(block);
}

DEF_AST_METHOD(ReturnStatement,AST_PRINT)
{
    PUTS("return ");
    if(expr)PRINTAST(expr);
    PUTS(";\n");
}

DEF_AST_METHOD(ContinueStatement,AST_PRINT)
{
    PUTS("continue;\n");
}

DEF_AST_METHOD(BreakStatement,AST_PRINT)
{
    PUTS("break;\n");
}

DEF_AST_METHOD(EmptyStatement,AST_PRINT)
{
    PUTS(";\n");
}

DEF_AST_METHOD(Program,AST_PRINT)
{
    PRINTAST(stmtList);
}
