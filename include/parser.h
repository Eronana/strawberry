#ifndef _PARSER_H
#define _PARSER_H

#include <vector>
#include <string>
#include <stack>
using namespace std;

#include "ast.h"
#include "lexer.h"

#define BINARY_OPERATOR_LIST(T) \
T(Multiplicative,Prefix)\
T(Additive,Multiplicative)\
T(Shift,Additive)\
T(Relational,Shift)\
T(Equality,Relational)\
T(BitwiseAND,Equality)\
T(BitwiseXOR,BitwiseAND)\
T(BitwiseOR,BitwiseXOR)\
T(LogicalAND,BitwiseOR)\
T(LogicalOR,LogicalAND)


#define BINARY_BOOLFUNC_LIST(T) \
T(BitwiseAND,TOKEN_BITWISE_AND)\
T(BitwiseXOR,TOKEN_BITWISE_XOR)\
T(BitwiseOR,TOKEN_BITWISE_OR)\
T(LogicalAND,TOKEN_LOGIC_AND)\
T(LogicalOR,TOKEN_LOGIC_OR)

#define NEW_AST_NAME(NAME) unique_ptr<AST_NAME(NAME)>(new AST_NAME(NAME))
#define DEF_AST_RET(NAME) auto ret=NEW_AST_NAME(NAME)
#define BINARY_OPERATOR_NAME(NAME) NAME##Expression
#define BINARY_BOOLFUNC_NAME(NAME) is##NAME##Operator

#define DECL_BINARY_OPERATOR(NAME,PRENAME) AST_PTR BINARY_OPERATOR_NAME(NAME)();
#define DECL_BINARY_BOOLFUNC(NAME,TOKEN) bool BINARY_BOOLFUNC_NAME(NAME)();

class Parser
{
  private:
    typedef bool(Parser:: *PARSER_BOOL_FUNC)();
    typedef AST_PTR (Parser::*PARSER_AST_FUNC)();

    Lexer &lexer;
    vector<Token> token_buffer;
    int token_base;
    Token &lookHead(int index = 0);
    void showToken(int num=5);
    bool accept(const string &s,int index=0);
    bool accept(TOKEN_TYPE t,int index=0);
    template<typename T,typename... Args>
    bool accept(const string &s,Args... args);
    template<typename T,typename... Args>
    bool accept(TOKEN_TYPE t,Args... args);
    void eatToken(int n=1);
    template<typename T>
    bool acceptEat(const T &x);
    int tokenStatus();
    void tokenStatus(int status);

    bool isLiteral();
    bool isUnaryOperator();
    bool isMultiplicativeOperator();
    bool isAdditiveOperator();
    bool isShiftOperator();
    bool isRelationalOperator();
    bool isEqualityOperator();
    bool isAssignmentOperator();
    BINARY_BOOLFUNC_LIST(DECL_BINARY_BOOLFUNC)

    AST_PTR Literal();
    AST_PTR NullLiteral();
    AST_PTR AssignmentExpression();
    AST_PTR ArrayLiteral();
    AST_PTR ObjectLiteral();
    AST_PTR PropertyNameAndValue();
    AST_PTR PrimaryExpression();
    AST_PTR MemberPartExpression();
    AST_PTR MemberPartIdentifer();
    AST_PTR MemberExpressionPartList();
    AST_PTR MemberExpression();
    AST_PTR ArgumentList();
    AST_PTR CallExpressionPartList();
    AST_PTR CallExpression(AST_PTR &&m);
    AST_PTR CallExpression();
    AST_PTR LeftHandSideExpression();
    AST_PTR PostfixExpression();
    AST_PTR PrefixExpression();
    AST_PTR Operation(PARSER_AST_FUNC ast_func);
    AST_PTR OperationList(PARSER_BOOL_FUNC bool_func,PARSER_AST_FUNC ast_func);
    BINARY_OPERATOR_LIST(DECL_BINARY_OPERATOR)
    AST_PTR ConditionalExpression();
    AST_PTR Expression();
    AST_PTR Block();
    AST_PTR VariableDeclaration();
    AST_PTR VariableStatement();
    AST_PTR ExpressionStatement();
    AST_PTR IfStatement();
    AST_PTR ForStatementInitial();
    AST_PTR ForStatement();
    AST_PTR DoStatement();
    AST_PTR WhileStatement();
    AST_PTR ReturnStatement();
    AST_PTR ContinueStatement();
    AST_PTR BreakStatement();
    AST_PTR CaseClause();
    AST_PTR CaseClauseList();
    AST_PTR CaseBlock();
    AST_PTR SwitchStatement();
    AST_PTR FormalParameterList();
    AST_PTR FunctionExpression();
    AST_PTR Statement();
    AST_PTR StatementList();
    AST_PTR Program();
  public:
    Parser(Lexer &lexer);
    AST_PTR parse();
};
#endif
