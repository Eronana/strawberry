#ifndef _AST_H_
#define _AST_H_

#include <memory>
#include <vector>
#include "narg.h"
#include "lexer.h"
#include "symbol.h"
#include "symbol.h"

using namespace std;

// to get child AST name
#define AST_NAME(NAME) AST_##NAME

// used to define ast method
#define DEF_AST_METHOD(NAME,FUNC) FUNC(AST_NAME(NAME)::)

// for declare base AST and children AST_XXX
#define DECL_AST_BASE_METHOD(FUNC) virtual FUNC=0;
#define DECL_AST_CHILD_METHOD(FUNC) FUNC;

// child ast header part
#define DECL_AST_HEADER(NAME) \
struct AST_NAME(NAME):AST { \
AST_METHOD_LIST(DECL_AST_CHILD_METHOD) \

// child ast footer part
#define DECL_AST_FOOTER };

// child ast fields part
#define DECL_AST_FIELDS(...) PP_JOIN(DECL_AST_FIELDS_,PP_NARG(__VA_ARGS__))(__VA_ARGS__)

// overload DECL_AST_FIELDS
#define DECL_AST_FIELDS_0() 
#define DECL_AST_FIELDS_1(F1) AST_PTR F1;
#define DECL_AST_FIELDS_2(F1,F2) DECL_AST_FIELDS_1(F1) DECL_AST_FIELDS_1(F2)
#define DECL_AST_FIELDS_3(F1,F2,F3) DECL_AST_FIELDS_2(F1,F2) DECL_AST_FIELDS_1(F3)
#define DECL_AST_FIELDS_4(F1,F2,F3,F4) DECL_AST_FIELDS_3(F1,F2,F3) DECL_AST_FIELDS_1(F4)
#define DECL_AST_FIELDS_5(F1,F2,F3,F4,F5) DECL_AST_FIELDS_4(F1,F2,F3,F4) DECL_AST_FIELDS_1(F5)

// declare child ast
#define DECL_AST(NAME,...) DECL_AST_HEADER(NAME) DECL_AST_FIELDS(__VA_ARGS__) DECL_AST_FOOTER

// declare array ast
#define DECL_ARRAY_AST(NAME) DECL_AST_HEADER(NAME) vector<AST_PTR> nodes; DECL_AST_FOOTER


// -------- AST as methods --------
// here is AST methods
#define AST_PRINT(NAME) void NAME print()
#define AST_FILL(NAME) void NAME fill(Symbol *symbol)
#define AST_CHECK(NAME) void NAME check(Symbol *symbol)
#define AST_CODEGEN(NAME) void NAME codeGen(Symbol *symbol)

// use above methods to define method list
#define AST_METHOD_LIST(T) \
T(AST_PRINT())\
T(AST_FILL())\
T(AST_CHECK())\
T(AST_CODEGEN())


// base AST
struct AST
{
    AST_METHOD_LIST(DECL_AST_BASE_METHOD)
    virtual ~AST()=0;
    static FILE *fp;
};

// use smart pointer to manage memory
typedef unique_ptr<AST> AST_PTR;

// -------- special AST --------
DECL_AST_HEADER(Literal)
Token token;
AST_NAME(Literal)(const Token &token);
AST_NAME(Literal)(int line,int col);
DECL_AST_FOOTER

// -------- array AST --------
DECL_ARRAY_AST(ArrayLiteral)
DECL_ARRAY_AST(ObjectLiteral)
DECL_ARRAY_AST(Expression)
DECL_ARRAY_AST(MemberExpressionPartList)
DECL_ARRAY_AST(ArgumentList)
DECL_ARRAY_AST(CallExpressionPartList)
DECL_ARRAY_AST(OperationList)
DECL_ARRAY_AST(CaseClauseList)
DECL_ARRAY_AST(FormalParameterList)
DECL_ARRAY_AST(StatementList)
DECL_ARRAY_AST(VariableDeclarationList)
DECL_ARRAY_AST(Block)

// -------- normal AST --------
DECL_AST(PrimaryExpression,expr)
DECL_AST(PropertyNameAndValue,identifier,assignExpr)
DECL_AST(MemberPartExpression,expr)
DECL_AST(MemberPartIdentifer,identifer)
DECL_AST(MemberExpression,expr,exprPart)
DECL_AST(CallExpression,expr,arguments,exprPart)
DECL_AST(PostfixExpression,expr,optr)
DECL_AST(PrefixExpression,expr,optr)
DECL_AST(BinaryOperationExpression,expr,optr)
DECL_AST(BinaryExpression,expr,operations)
DECL_AST(ConditionalExpression,expr,trueExpr,falseExpr)
DECL_AST(AssignmentExpression,expr,assignOptr,assignExpr)
DECL_AST(ExpressionStatement,expr)
DECL_AST(IfStatement,expr,statement,elseStatement)
DECL_AST(VariableDeclaration,identifier,assignExpr)
DECL_AST(ForStatement,init,expr,loopExpr,statement)
DECL_AST(DoStatement,statement,expr)
DECL_AST(WhileStatement,expr,statement)
DECL_AST(CaseClause,expr,stmtList)
DECL_AST(CaseBlock,caseClausesList)
DECL_AST(SwitchStatement,expr,caseBlock)
DECL_AST(FunctionExpression,formalParameterList,block)
DECL_AST(ReturnStatement,literal,expr)
DECL_AST(ContinueStatement,literal)
DECL_AST(BreakStatement,literal)
DECL_AST(EmptyStatement)
DECL_AST(Program,stmtList)
#endif
