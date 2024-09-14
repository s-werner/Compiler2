#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    INTEGER,
    FLOAT,
    IDENTIFIER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULUS,
    POWER,
    ASSIGN,
    SEMICOLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

#endif // TOKEN_H
