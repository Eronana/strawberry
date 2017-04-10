
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "lexer.h"
#include <string>
using namespace std;
class Exception
{
private:
    Token token;
    std::string err;
public:
    Exception(const Token &token,const std::string &err);
    void print(FILE *fp);
};

#define CHECK_ERROR(T,S) throw Exception(T,S)
#define PARSER_ERROR(s) throw Exception(lookHead(),s)
#endif
