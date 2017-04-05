#include "lexer.h"

const char *TOKEN_NAME[] = {TOKEN_LIST(GET_TOKEN_NAME)};
const char CHAR_EOF=(char)EOF;

void Token::print(FILE *fp)
{
    fprintf(fp, "[TOKEN: %s, RAW: \"%s\", LINE: %d, COL: %d", TOKEN_NAME[type], raw.c_str(), line, col);
    if (msg.length()) fprintf(fp, ", MSG: %s", msg.c_str());
    fputs("]\n", fp);
}

char Lexer::lookHead(int index)
{
    while (index >= lookhead_buffer.size()) lookhead_buffer.push((char)fgetc(fp));
    return lookhead_buffer[index];
}

void Lexer::clearChar(int len)
{
    col_number += len;
    lookhead_buffer.pop(len);
}

bool Lexer::isNumber(char c)
{
    return c >= '0' && c <= '9';
}
bool Lexer::isHex(char c)
{
    return isNumber(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
bool Lexer::isOct(char c)
{
    return c >= '0' && c <= '7';
}
bool Lexer::isBin(char c)
{
    return c == '0' || c == '1';
}
bool Lexer::isIdentifierHead(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_');
}
bool Lexer::isIdentifier(char c)
{
    return isIdentifierHead(c) || isNumber(c);
}
bool Lexer::isWhiteSpace(char c)
{
    for (const char *w = " \r\f\t\v"; *w; w++) if (*w == c) return true;
    return false;
}

Token Lexer::createToken(TOKEN_TYPE type, int len = 1)
{
    Token t;
    t.type = type;
    t.line = line_number;
    t.col = col_number;
    t.raw = std::string(lookhead_buffer.begin(), len);
    clearChar(len);
    return t;
}
Token Lexer::createToken(TOKEN_TYPE type, int len, const std::string &s)
{
    Token t = createToken(type, len);
    t.msg = s;
    return t;
}
void Lexer::skipWhiteSpace()
{
    for (char c; ~(c = lookHead()); clearChar())
    {
        if (c == '\n') line_number++, col_number = 0;
        else if (!isWhiteSpace(c)) break;
    }
}
bool Lexer::skipComment(Token &t)
{
    skipWhiteSpace();
    if (lookHead() != '/') return true;
    if (lookHead(1) == '/')
    {
        for (clearChar(2); lookHead() != CHAR_EOF && lookHead() != '\n'; clearChar());
        if (lookHead() == CHAR_EOF) return true;
        line_number++;
        col_number = 0;
        clearChar();
        return skipComment(t);
    }

    else if (lookHead(1) == '*')
    {
        int comment_nest = 1;
        for (clearChar(2); comment_nest; clearChar())
        {
            switch (lookHead())
            {
            case CHAR_EOF:
                t = createToken(TOKEN_ERROR, 0, "Unterminated comment");
                return false;
            case '\n':
                line_number++;
                col_number = 0;
                break;
            case '/':
                if (lookHead(1) == '*')
                {
                    comment_nest++;
                    clearChar();
                }
                else if (lookHead(1) != '/') clearChar();
                break;
            case '*':
                if (lookHead(1) == '/')
                {
                    comment_nest--;
                    clearChar();
                }
                else if (lookHead(1) != '*') clearChar();
                break;
            }
        }
        return skipComment(t);
    }
    return true;
}

bool Lexer::load(const char *file)
{
    fp = fopen(file, "r");
    if (!fp) return false;
    line_number = 1;
    col_number = 1;
    return true;
}

Lexer::~Lexer()
{
    if (fp) fclose(fp);
}

Token Lexer::next()
{
    if (!fp) return createToken(TOKEN_ERROR, 0, "No source file.");
    Token comment_error, t;
    if (!skipComment(comment_error)) return comment_error;
    skipWhiteSpace();
    int c, i;
    if ((c = lookHead()) == CHAR_EOF) return createToken(TOKEN_EOF, 0);
    switch (c)
    {
    case '+':
        switch (lookHead(1))
        {
        case '+':
            return createToken(TOKEN_INCREMENT, 2);
        case '=':
            return createToken(TOKEN_ADD_ASSIGN, 2);
        default:
            return createToken(TOKEN_ADD);
        }
    case '-':
        switch (lookHead(1))
        {
        case '-':
            return createToken(TOKEN_DECREMENT, 2);
        case '=':
            return createToken(TOKEN_SUB_ASSIGN, 2);
        default:
            return createToken(TOKEN_SUB);
        }
    case '*':
        if (lookHead(1) == '=') return createToken(TOKEN_MUL_ASSIGN, 2);
        return createToken(TOKEN_MUL);
    case '/':
        if (lookHead(1) == '=') return createToken(TOKEN_DIV_ASSIGN, 2);
        return createToken(TOKEN_DIV);
    case '%':
        if (lookHead(1) == '=') return createToken(TOKEN_MOD_ASSIGN, 2);
        return createToken(TOKEN_MOD);
    case '(':
        return createToken(TOKEN_LEFT_PAREN);
    case ')':
        return createToken(TOKEN_RIGHT_PAREN);
    case '[':
        return createToken(TOKEN_LEFT_SQUARE_BARCKET);
    case ']':
        return createToken(TOKEN_RIGHT_SQUARE_BARCKET);
    case '{':
        return createToken(TOKEN_LEFT_BARCKET);
    case '}':
        return createToken(TOKEN_RIGHT_BARCKET);
    case '|':
        switch (lookHead(1))
        {
        case '|':
            return createToken(TOKEN_LOGIC_OR, 2);
        case '=':
            return createToken(TOKEN_BITWISE_OR_ASSIGN, 2);
        default:
            return createToken(TOKEN_BITWISE_OR);
        }
    case '&':
        switch (lookHead(1))
        {
        case '&':
            return createToken(TOKEN_LOGIC_AND, 2);
        case '=':
            return createToken(TOKEN_BITWISE_AND_ASSIGN, 2);
        default:
            return createToken(TOKEN_BITWISE_AND);
        }
    case '~':
        return createToken(TOKEN_BITWISE_NOT);
    case '^':
        if (lookHead(1) == '=') return createToken(TOKEN_BITWISE_XOR_ASSIGN, 2);
        return createToken(TOKEN_BITWISE_XOR);
    case '!':
        if (lookHead(1) == '=') return createToken(TOKEN_INEQUAL, 2);
        return createToken(TOKEN_NEGATION);
    case '=':
        if (lookHead(1) == '=') return createToken(TOKEN_EQUAL, 2);
        return createToken(TOKEN_ASSIGN);
    case '>':
        if (lookHead(1) == '>')
        {
            if (lookHead(2) == '=') return createToken(TOKEN_RIGHT_SHIFT_ASSIGN, 3);
            return createToken(TOKEN_RIGHT_SHIFT, 2);
        }
        else if (lookHead(1) == '=') return createToken(TOKEN_GREATER_EQUAL, 2);
        return createToken(TOKEN_GREATER);
    case '<':
        if (lookHead(1) == '<')
        {
            if (lookHead(2) == '=') return createToken(TOKEN_LEFT_SHIFT_ASSIGN, 3);
            return createToken(TOKEN_LEFT_SHIFT, 2);
        }
        else if (lookHead(1) == '=') return createToken(TOKEN_LESS_EQUAL, 2);
        return createToken(TOKEN_LESS);
    case ';':
        return createToken(TOKEN_SEMICOLON);
    case ',':
        return createToken(TOKEN_COMMA);
    case ':':
        return createToken(TOKEN_COLON);
    case '?':
        return createToken(TOKEN_QUESTIONMARK);
    case '\'':
    case '"':
        for (i = 1; (lookHead(i) != c || lookHead(i - 1) == '\\') && lookHead(i) != '\n' && lookHead(i) != CHAR_EOF; i++);
        if (lookHead(i) == '\n') return createToken(TOKEN_ERROR, i, "Unterminated string literal in new line");
        if (lookHead(i) == CHAR_EOF) return createToken(TOKEN_ERROR, i, "Unterminated string literal in EOF");
        return createToken(TOKEN_STRING_LITERAL, i + 1);
    default:
        if (isIdentifierHead(c))
        {
            for (i = 1; isIdentifier(lookHead(i)); i++);
            t = createToken(TOKEN_IDENTIFIER, i);
            if (t.raw == "true" || t.raw == "false") t.type = TOKEN_BOOL_LITERAL;
            if (t.raw == "null") t.type = TOKEN_NULL_LITERAL;
            return t;
        }
        if (c == '0')
            switch (lookHead(1))
            {
            case 'b':
            case 'B':
                for (i = 2; isBin(lookHead(i)); i++);
                return createToken(TOKEN_BIN_INTEGER_LITERAL, i);
            case 'x':
            case 'X':
                for (i = 2; isHex(lookHead(i)); i++);
                return createToken(TOKEN_HEX_INTEGER_LITERAL, i);
            case '.':
                break;
            default:
                for (i = 1; isOct(lookHead(i)); i++);
                return createToken(TOKEN_OCT_INTEGER_LITERAL, i);
            }
        if (c == '.')
        {
            if (isNumber(lookHead(1)))
            {
                for (i = 1; isNumber(lookHead(i)); i++);
                if (lookHead(i) == 'F' || lookHead(i) == 'f') return createToken(TOKEN_FLOAT_LITERAL, i + 1);
                return createToken(TOKEN_FLOAT_LITERAL, i);
            }
            else
                return createToken(TOKEN_DOT);
        }
        if (isNumber(c))
        {
            for (i = 1; isNumber(lookHead(i)); i++);
            if (lookHead(i) != '.')
            {
                if (lookHead(i) == 'F' || lookHead(i) == 'f') return createToken(TOKEN_FLOAT_LITERAL, i + 1);
                return createToken(TOKEN_DEC_INTEGER_LITERAL, i);
            }
            for (i++; isNumber(lookHead(i)); i++);
            if (lookHead(i) == 'F' || lookHead(i) == 'f') i++;
            return createToken(TOKEN_FLOAT_LITERAL, i);
        }
        return createToken(TOKEN_ERROR, 1, "Unexcepted symbol");
    }
}
