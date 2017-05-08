#ifndef _LEXER_H_
#define _LEXER_H_
#include <string>
#include "token_list.h"
#include "queue.hpp"

#define GET_TOKEN_LIST(NAME) TOKEN_##NAME,
#define GET_TOKEN_NAME(NAME) #NAME,
enum TOKEN_TYPE
{
    TOKEN_LIST(GET_TOKEN_LIST)
};

struct Token
{
    TOKEN_TYPE type;
    int line, col;
    std::string raw, msg;
    void print(FILE *fp);
};
class Lexer
{
  private:
    FILE *fp=NULL;
    Queue<char> lookhead_buffer;
    int line_number;
    int col_number;
    char lookHead(int index = 0);
    void clearChar(int len = 1);
    bool isNumber(char c);
    bool isHex(char c);
    bool isOct(char c);
    bool isBin(char c);
    bool isIdentifierHead(char c);
    bool isIdentifier(char c);
    bool isWhiteSpace(char c);
    Token createToken(TOKEN_TYPE type, int len);
    Token createToken(TOKEN_TYPE type, int len, const std::string &s);
    void skipWhiteSpace();
    bool skipComment(Token &t);

  public:
    bool load(const char *file);
    ~Lexer();
    Token next();
};
#endif
