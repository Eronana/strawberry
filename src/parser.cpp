#include "parser.h"
#include "exception.h"

#define RETURN_PTR return move(ret)
#define DEF_BINARY_OPERATOR(NAME,PRENAME) \
AST_PTR Parser::BINARY_OPERATOR_NAME(NAME)() \
{\
    AST_PTR expr=BINARY_OPERATOR_NAME(PRENAME)();\
    if(BINARY_BOOLFUNC_NAME(NAME)())\
    {\
        DEF_AST_RET(BinaryExpression);\
        ret->expr=move(expr);\
        ret->operations=OperationList(&Parser::BINARY_BOOLFUNC_NAME(NAME),&Parser::BINARY_OPERATOR_NAME(PRENAME));\
        RETURN_PTR;\
    }\
    return expr;\
}

#define DEF_BINARY_BOOLFUNC(NAME,TOKEN) \
bool Parser::BINARY_BOOLFUNC_NAME(NAME)()\
{\
    return accept(TOKEN);\
}


int countArgs()
{
    return 0;
}
template<typename T,typename... Args>
int countArgs(T,Args... args)
{
    return 1+countArgs(args...);
}


Token &Parser::lookHead(int index)
{
    while(token_base+index>=token_buffer.size())token_buffer.push_back(lexer.next());
    return token_buffer[token_base+index];
}
void Parser::showToken(int num)
{
    for(int i=0;i<num;i++)puts(lookHead(i).raw.c_str());
}
bool Parser::accept(const string &s,int index)
{
    return s==lookHead(index).raw;
}
bool Parser::accept(TOKEN_TYPE t,int index)
{
    return t==lookHead(index).type;
}
template<typename T,typename... Args>
bool Parser::accept(const std::string &s,Args... args)
{
    if(!accept(s,countArgs(args...)))return false;
    return accept(args...);
}
template<typename T,typename... Args>
bool Parser::accept(TOKEN_TYPE t,Args... args)
{
    if(!accept(t,countArgs(args...)))return false;
    return accept(args...);
}
void Parser::eatToken(int n)
{
    token_base+=n;
}
template<typename T>
bool Parser::acceptEat(const T &x)
{
    if(accept(x))return token_base++,true;
    return false;
}

int Parser::tokenStatus()
{
    return token_base;
}
void Parser::tokenStatus(int status)
{
    token_base=status;
}

bool Parser::isLiteral()
{
    return accept(TOKEN_BIN_INTEGER_LITERAL)||
    accept(TOKEN_OCT_INTEGER_LITERAL)||
    accept(TOKEN_DEC_INTEGER_LITERAL)||
    accept(TOKEN_HEX_INTEGER_LITERAL)||
    accept(TOKEN_FLOAT_LITERAL)||
    accept(TOKEN_STRING_LITERAL)||
    accept(TOKEN_BOOL_LITERAL)||
    accept(TOKEN_NULL_LITERAL)||
    accept(TOKEN_IDENTIFIER);

}

bool Parser::isUnaryOperator()
{
    return accept("typeof")||
    accept(TOKEN_INCREMENT)||
    accept(TOKEN_DECREMENT)||
    accept(TOKEN_ADD)||
    accept(TOKEN_SUB)||
    accept(TOKEN_BITWISE_NOT)||
    accept(TOKEN_NEGATION);
}

bool Parser::isMultiplicativeOperator()
{
    return accept(TOKEN_MUL)||
    accept(TOKEN_DIV)||
    accept(TOKEN_MOD);
}
bool Parser::isAdditiveOperator()
{
    return accept(TOKEN_ADD)||accept(TOKEN_SUB);
}

bool Parser::isShiftOperator()
{
    return accept(TOKEN_LEFT_SHIFT)||accept(TOKEN_RIGHT_SHIFT);
}

bool Parser::isRelationalOperator()
{
    return accept(TOKEN_LESS)||
    accept(TOKEN_GREATER)||
    accept(TOKEN_LESS_EQUAL)||
    accept(TOKEN_GREATER_EQUAL);
}

bool Parser::isEqualityOperator()
{
    return accept(TOKEN_EQUAL)||accept(TOKEN_INEQUAL);
}

bool Parser::isAssignmentOperator()
{
    return accept(TOKEN_ASSIGN)||
    accept(TOKEN_MUL_ASSIGN)||
    accept(TOKEN_DIV_ASSIGN)||
    accept(TOKEN_MOD_ASSIGN)||
    accept(TOKEN_ADD_ASSIGN)||
    accept(TOKEN_SUB_ASSIGN)||
    accept(TOKEN_LEFT_SHIFT_ASSIGN)||
    accept(TOKEN_RIGHT_SHIFT_ASSIGN)||
    accept(TOKEN_BITWISE_AND_ASSIGN)||
    accept(TOKEN_BITWISE_XOR_ASSIGN)||
    accept(TOKEN_BITWISE_OR_ASSIGN);
}


BINARY_BOOLFUNC_LIST(DEF_BINARY_BOOLFUNC)
BINARY_OPERATOR_LIST(DEF_BINARY_OPERATOR)

AST_PTR Parser::Literal()
{
    auto ret=unique_ptr<AST_NAME(Literal)>(new AST_NAME(Literal)(lookHead()));
    eatToken();
    RETURN_PTR;
}
AST_PTR Parser::NullLiteral()
{
    Token &t=lookHead();
    return unique_ptr<AST_NAME(Literal)>(new AST_NAME(Literal)(t.line,t.col));
}

AST_PTR Parser::AssignmentExpression()
{
    int status=tokenStatus();
    try
    {
        AST_PTR expr=LeftHandSideExpression();
        if(isAssignmentOperator())
        {
            DEF_AST_RET(AssignmentExpression);
            ret->expr=move(expr);
            ret->assignOptr=Literal();
            ret->assignExpr=AssignmentExpression();
            RETURN_PTR;
        }
    }
    catch(Exception){}
    tokenStatus(status);
    return ConditionalExpression();
}

AST_PTR Parser::ArrayLiteral()
{
    if(!acceptEat(TOKEN_LEFT_SQUARE_BARCKET))PARSER_ERROR("exception a array literal start with '['");
    DEF_AST_RET(ArrayLiteral);
    while(!accept(TOKEN_RIGHT_SQUARE_BARCKET))
    {
        if(accept(TOKEN_COMMA))
        {
            ret->nodes.push_back(NullLiteral());
            eatToken();
        }
        else
        {
            ret->nodes.push_back(AssignmentExpression());
            if(!acceptEat(TOKEN_COMMA)&&!accept(TOKEN_RIGHT_SQUARE_BARCKET))PARSER_ERROR("excepted a ',' after array element");
        }
    }
    eatToken();
    RETURN_PTR;
}

AST_PTR Parser::ObjectLiteral()
{
    if(!acceptEat(TOKEN_LEFT_BARCKET))PARSER_ERROR("exception a object literal start with '{'");
    DEF_AST_RET(ObjectLiteral);
    while(!accept(TOKEN_RIGHT_BARCKET))
    {
        ret->nodes.push_back(PropertyNameAndValue());
        if(!acceptEat(TOKEN_COMMA)&&!accept(TOKEN_RIGHT_BARCKET))PARSER_ERROR("excepted a '}' after object element");
    }
    eatToken();
    RETURN_PTR;
}

AST_PTR Parser::PropertyNameAndValue()
{
    if(!accept(TOKEN_STRING_LITERAL)&&!accept(TOKEN_IDENTIFIER))PARSER_ERROR("exception a identifier or string literal");
    DEF_AST_RET(PropertyNameAndValue);
    ret->identifier=Literal();
    if(!acceptEat(TOKEN_COLON))PARSER_ERROR("exception a ':'");
    ret->assignExpr=AssignmentExpression();
    RETURN_PTR;
}

AST_PTR Parser::PrimaryExpression()
{
    AST_PTR ret;
    if(acceptEat(TOKEN_LEFT_PAREN))
    {
        DEF_AST_RET(PrimaryExpression);
        ret->expr=Expression();
        if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
        RETURN_PTR;
    }
    else if(isLiteral())
    {
        ret=Literal();
    }
    else if(accept(TOKEN_LEFT_BARCKET))
    {
        ret=ObjectLiteral();
    }
    else if(accept(TOKEN_LEFT_SQUARE_BARCKET))
    {
        ret=ArrayLiteral();
    }
    if(!ret)PARSER_ERROR("Exception Object, Array, (Expression), Identifier or Literal");
    return ret;
}

AST_PTR Parser::MemberPartExpression()
{
    if(!acceptEat(TOKEN_LEFT_SQUARE_BARCKET))return nullptr;
    DEF_AST_RET(MemberPartExpression);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_RIGHT_SQUARE_BARCKET))PARSER_ERROR("excepted ']'");
    RETURN_PTR;
}

AST_PTR Parser::MemberPartIdentifer()
{
    if(!acceptEat(TOKEN_DOT))return nullptr;
    if(!accept(TOKEN_IDENTIFIER))PARSER_ERROR("excepted a identifier after '.'");
    DEF_AST_RET(MemberPartIdentifer);
    ret->identifer=Literal();
    RETURN_PTR;
}

AST_PTR Parser::MemberExpressionPartList()
{
    DEF_AST_RET(MemberExpressionPartList);
    while(true)
    {
        AST_PTR part=MemberPartIdentifer();
        if(!part)part=MemberPartExpression();
        if(!part)break;
        ret->nodes.push_back(move(part));
    }
    RETURN_PTR;
}

AST_PTR Parser::MemberExpression()
{
    AST_PTR expr;
    if(accept("function"))expr=FunctionExpression();
    else expr=PrimaryExpression();
    if(accept(TOKEN_DOT)||accept(TOKEN_LEFT_SQUARE_BARCKET))
    {
        DEF_AST_RET(MemberExpression);
        ret->expr=move(expr);
        ret->exprPart=MemberExpressionPartList();
        RETURN_PTR;
    }
    else return expr;
}

AST_PTR Parser::ArgumentList()
{
    if(!acceptEat(TOKEN_LEFT_PAREN))return nullptr;
    DEF_AST_RET(ArgumentList);
    while(!accept(TOKEN_RIGHT_PAREN))
    {
        ret->nodes.push_back(AssignmentExpression());
        if(!acceptEat(",")&&!accept(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted ')'");
    }
    eatToken();
    RETURN_PTR;
}

AST_PTR Parser::CallExpressionPartList()
{
    DEF_AST_RET(CallExpressionPartList);
    while(true)
    {
        AST_PTR part=MemberPartIdentifer();
        if(!part)part=MemberPartExpression();
        if(!part)part=ArgumentList();
        if(!part)break;
        ret->nodes.push_back(move(part));
    }
    if(ret->nodes.size()==0)return nullptr;
    RETURN_PTR;
}

AST_PTR Parser::CallExpression(AST_PTR &&m)
{
    DEF_AST_RET(CallExpression);
    ret->expr=move(m);
    if(!accept(TOKEN_LEFT_PAREN))PARSER_ERROR("excepted '('");
    ret->arguments=ArgumentList();
    ret->exprPart=CallExpressionPartList();
    RETURN_PTR;
}

AST_PTR Parser::CallExpression()
{
    return CallExpression(MemberExpression());
}

AST_PTR Parser::LeftHandSideExpression()
{
    AST_PTR ret=MemberExpression();
    if(accept(TOKEN_LEFT_PAREN))return CallExpression(move(ret));
    return ret;
}

AST_PTR Parser::PostfixExpression()
{
    AST_PTR expr=LeftHandSideExpression();
    if(accept(TOKEN_INCREMENT)||accept(TOKEN_DECREMENT))
    {
        DEF_AST_RET(PostfixExpression);
        ret->expr=move(expr);
        ret->optr=Literal();
        RETURN_PTR;
    }
    else return expr;
}

AST_PTR Parser::PrefixExpression()
{
    if(isUnaryOperator())
    {
        DEF_AST_RET(PrefixExpression);
        ret->optr=Literal();
        ret->expr=PrefixExpression();
        RETURN_PTR;
    }
    else return PostfixExpression();
}

AST_PTR Parser::Operation(PARSER_AST_FUNC ast_func)
{
    DEF_AST_RET(BinaryOperationExpression);
    ret->optr=Literal();
    ret->expr=(this->*ast_func)();
    RETURN_PTR;
}

AST_PTR Parser::OperationList(PARSER_BOOL_FUNC bool_func,PARSER_AST_FUNC ast_func)
{
    DEF_AST_RET(OperationList);
    do
    {
        ret->nodes.push_back(Operation(ast_func));
    }while((this->*bool_func)());
    RETURN_PTR;
}


AST_PTR Parser::ConditionalExpression()
{
    AST_PTR expr=LogicalORExpression();
    if(acceptEat(TOKEN_QUESTIONMARK))
    {
        DEF_AST_RET(ConditionalExpression);
        ret->expr=move(expr);
        ret->trueExpr=AssignmentExpression();
        if(!acceptEat(TOKEN_COLON))PARSER_ERROR("excepted ':' for conditional expression");
        ret->falseExpr=AssignmentExpression();
        RETURN_PTR;
    }
    else return expr;
}


AST_PTR Parser::Expression()
{
    AST_PTR expr=AssignmentExpression();
    if(acceptEat(TOKEN_COMMA))
    {
        DEF_AST_RET(Expression);
        ret->nodes.push_back(move(expr));
        do ret->nodes.push_back(AssignmentExpression()); while(acceptEat(TOKEN_COMMA));
        RETURN_PTR;
    }
    else return expr;
}

AST_PTR Parser::Block()
{
    if(!acceptEat(TOKEN_LEFT_BARCKET))PARSER_ERROR("expected a '{'");
    auto stmtlist=StatementList();
    DEF_AST_RET(Block);
    ret->nodes.swap(((AST_NAME(StatementList)*)stmtlist.get())->nodes);
    if(!acceptEat(TOKEN_RIGHT_BARCKET))Statement();
    RETURN_PTR;
}

AST_PTR Parser::VariableDeclaration()
{
    
    if(!accept(TOKEN_IDENTIFIER))PARSER_ERROR("expected a identifier");
    DEF_AST_RET(VariableDeclaration);
    ret->identifier=Literal();
    if(acceptEat(TOKEN_ASSIGN))ret->assignExpr=AssignmentExpression();
    RETURN_PTR;
}

AST_PTR Parser::VariableStatement()
{
    if(!acceptEat("var"))PARSER_ERROR("expected a 'var'");
    DEF_AST_RET(VariableDeclarationList);
    do ret->nodes.push_back(VariableDeclaration());while(acceptEat(TOKEN_COMMA));
    if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("expected a ';'");
    RETURN_PTR;
}

AST_PTR Parser::ExpressionStatement()
{
    DEF_AST_RET(ExpressionStatement);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    RETURN_PTR;
}

AST_PTR Parser::IfStatement()
{
    if(!acceptEat("if"))PARSER_ERROR("excepted 'if'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("expected a '('");
    DEF_AST_RET(IfStatement);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("expected a ')'");
    ret->statement=Statement();
    if(acceptEat("else"))ret->elseStatement=Statement();
    RETURN_PTR;
}

AST_PTR Parser::ForStatementInitial()
{
    if(accept("var"))return VariableStatement();
    else if(acceptEat(TOKEN_SEMICOLON))return AST_PTR(new AST_NAME(EmptyStatement)); 
    else return ExpressionStatement();
}

AST_PTR Parser::ForStatement()
{
    if(!acceptEat("for"))PARSER_ERROR("excepted 'for'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("expected a '('");
    DEF_AST_RET(ForStatement);
    ret->init=ForStatementInitial();
    if(!acceptEat(TOKEN_SEMICOLON))
    {
        ret->expr=Expression();
        if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    }
    if(!acceptEat(TOKEN_RIGHT_PAREN))
    {
        ret->loopExpr=Expression();
        if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
    }
    ret->statement=Statement();
    RETURN_PTR;
}

AST_PTR Parser::DoStatement()
{
    if(!acceptEat("do"))PARSER_ERROR("excepted a 'do'");
    DEF_AST_RET(DoStatement);
    ret->statement=Statement();
    if(!acceptEat("while"))PARSER_ERROR("excepted a 'while'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("excepted a '('");
    ret->expr=Expression();
    if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
    if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    RETURN_PTR;
}

AST_PTR Parser::WhileStatement()
{
    if(!acceptEat("while"))PARSER_ERROR("excepted a 'while'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("excepted a '('");
    DEF_AST_RET(WhileStatement);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
    ret->statement=Statement();
    RETURN_PTR;
}

AST_PTR Parser::ReturnStatement()
{
    if(!accept("return"))PARSER_ERROR("excepted a 'return'");
    DEF_AST_RET(ReturnStatement);
    ret->literal=Literal();
    if(!acceptEat(TOKEN_SEMICOLON))
    {
        ret->expr=Expression();
        if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    }
    RETURN_PTR;
}
AST_PTR Parser::ContinueStatement()
{
    if(!accept("continue"))PARSER_ERROR("excepted a 'continue'");
    DEF_AST_RET(ContinueStatement);
    ret->literal=Literal();
    if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    RETURN_PTR;
}
AST_PTR Parser::BreakStatement()
{
    if(!accept("break"))PARSER_ERROR("excepted a 'break'");
    DEF_AST_RET(BreakStatement);
    ret->literal=Literal();
    if(!acceptEat(TOKEN_SEMICOLON))PARSER_ERROR("excepted a ';'");
    RETURN_PTR;
}

AST_PTR Parser::CaseClause()
{
    if(!acceptEat("case"))PARSER_ERROR("expected a 'case'");
    DEF_AST_RET(CaseClause);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_COLON))PARSER_ERROR("expected a ':'");
    ret->stmtList=StatementList();
    RETURN_PTR;
}

AST_PTR Parser::CaseClauseList()
{
    DEF_AST_RET(CaseClauseList);
    while(accept("case"))ret->nodes.push_back(CaseClause());
    if(acceptEat("default"))
    {
        if(!acceptEat(TOKEN_COLON))PARSER_ERROR("excepted a ':' after 'default'");
        auto defaultClause=NEW_AST_NAME(CaseClause);
        defaultClause->stmtList=StatementList();
        ret->nodes.push_back(move(defaultClause));
        while(accept("case"))ret->nodes.push_back(CaseClause());
    }
    RETURN_PTR;
}

AST_PTR Parser::CaseBlock()
{
    if(!acceptEat(TOKEN_LEFT_BARCKET))PARSER_ERROR("expected a '{'");
    DEF_AST_RET(CaseBlock);
    ret->caseClausesList=CaseClauseList();
    if(!acceptEat(TOKEN_RIGHT_BARCKET))PARSER_ERROR("expected a '}'");
    RETURN_PTR;
}

AST_PTR Parser::SwitchStatement()
{
    if(!acceptEat("switch"))PARSER_ERROR("excepted a 'switch'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("excepted a '('");
    DEF_AST_RET(SwitchStatement);
    ret->expr=Expression();
    if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
    ret->caseBlock=CaseBlock();
    RETURN_PTR;
}

AST_PTR VirtualLiteral(const string &s)
{
    auto ret=unique_ptr<AST_NAME(Literal)>(new AST_NAME(Literal)({TOKEN_IDENTIFIER,0,0,s}));
    RETURN_PTR;
}

AST_PTR Parser::FormalParameterList()
{
    DEF_AST_RET(FormalParameterList);
    if(accept(TOKEN_IDENTIFIER))do
    {
        if(!accept(TOKEN_IDENTIFIER))PARSER_ERROR("excepted a identifier");
        ret->nodes.push_back(Literal());
    }while(acceptEat(TOKEN_COMMA));
    ret->nodes.push_back(VirtualLiteral("this"));
    ret->nodes.push_back(VirtualLiteral("callee"));
    RETURN_PTR;
}

AST_PTR Parser::FunctionExpression()
{
    if(!acceptEat("function"))PARSER_ERROR("excepted a 'function'");
    if(!acceptEat(TOKEN_LEFT_PAREN))PARSER_ERROR("excepted a '('");
    DEF_AST_RET(FunctionExpression);
    ret->formalParameterList=FormalParameterList();
    if(!acceptEat(TOKEN_RIGHT_PAREN))PARSER_ERROR("excepted a ')'");
    ret->block=Block();
    RETURN_PTR;
}

AST_PTR Parser::Statement()
{
    if(accept(TOKEN_LEFT_BARCKET)) return Block();
    else if(accept("var"))return VariableStatement();
    else if(acceptEat(TOKEN_SEMICOLON))return AST_PTR(new AST_NAME(EmptyStatement));
    else if(accept("if"))return IfStatement();
    else if(accept("for"))return ForStatement();
    else if(accept("do"))return DoStatement();
    else if(accept("while"))return WhileStatement();
    else if(accept("switch"))return SwitchStatement();
    else if(accept("return"))return ReturnStatement();
    else if(accept("continue"))return ContinueStatement();
    else if(accept("break"))return BreakStatement();
    else return ExpressionStatement();
}

AST_PTR Parser::StatementList()
{
    DEF_AST_RET(StatementList);
    int status;
    while(!accept(TOKEN_EOF))
    {
        status=tokenStatus();
        try
        {
            ret->nodes.push_back(Statement());
        }
        catch(Exception)
        {
            tokenStatus(status);
            break;
        }
    }
    RETURN_PTR;
}

AST_PTR Parser::Program()
{
    DEF_AST_RET(Program);
    ret->stmtList=StatementList();
    if(accept(TOKEN_EOF))RETURN_PTR;
    if(accept(TOKEN_ERROR))PARSER_ERROR(lookHead().msg);
    Statement();
    return nullptr;
}

Parser::Parser(Lexer &lexer):lexer(lexer),token_base(0){}

AST_PTR Parser::parse()
{
    return Program();
}
