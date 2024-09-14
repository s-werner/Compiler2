#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer {
public:
    Lexer(const std::string& text);
    Token getNextToken();
private:
    std::string text;
    size_t pos;
    char currentChar;

    void advance();
    void skipWhitespace();
    Token integer();
    Token number();
    Token identifier();
};

#endif // LEXER_H
