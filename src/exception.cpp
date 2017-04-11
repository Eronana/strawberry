#include "exception.h"

Exception::Exception(const Token &token,const string &err):token(token),err(err){}
void Exception::print(FILE *fp)
{
    fprintf(fp,"Line: %d, Column: %d, Token: %s. Error: %s\n",token.line,token.col,token.raw.c_str(),err.c_str());
}
