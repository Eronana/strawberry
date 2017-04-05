#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#define TOKEN_LIST_BEGIN 
#define TOKEN_LIST_END 

#define TOKEN_LIST(f) \
TOKEN_LIST_END \
f(ERROR)\
f(EOF)\
f(INCREMENT)\
f(ADD_ASSIGN)\
f(ADD)\
f(DECREMENT)\
f(SUB_ASSIGN)\
f(SUB)\
f(MUL_ASSIGN)\
f(MUL)\
f(DIV_ASSIGN)\
f(DIV)\
f(MOD_ASSIGN)\
f(MOD)\
f(LEFT_PAREN)\
f(RIGHT_PAREN)\
f(LEFT_SQUARE_BARCKET)\
f(RIGHT_SQUARE_BARCKET)\
f(LEFT_BARCKET)\
f(RIGHT_BARCKET)\
f(LOGIC_OR)\
f(BITWISE_OR_ASSIGN)\
f(BITWISE_OR)\
f(LOGIC_AND)\
f(BITWISE_AND_ASSIGN)\
f(BITWISE_AND)\
f(BITWISE_NOT)\
f(BITWISE_XOR_ASSIGN)\
f(BITWISE_XOR)\
f(INEQUAL)\
f(NEGATION)\
f(EQUAL)\
f(ASSIGN)\
f(RIGHT_SHIFT_ASSIGN)\
f(RIGHT_SHIFT)\
f(GREATER)\
f(GREATER_EQUAL)\
f(LEFT_SHIFT_ASSIGN)\
f(LEFT_SHIFT)\
f(LESS)\
f(LESS_EQUAL)\
f(SEMICOLON)\
f(COMMA)\
f(DOT)\
f(COLON)\
f(QUESTIONMARK)\
f(IDENTIFIER)\
f(STRING_LITERAL)\
f(BIN_INTEGER_LITERAL)\
f(OCT_INTEGER_LITERAL)\
f(DEC_INTEGER_LITERAL)\
f(HEX_INTEGER_LITERAL)\
f(BOOL_LITERAL)\
f(NULL_LITERAL)\
f(FLOAT_LITERAL) \
TOKEN_LIST_END

#endif