#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    END_OF_FILE,    // 0
    INTEGER,        // 1
    FLOAT,          // 2
    IDENTIFIER,     // 3
    ASSIGN,         // 4
    PLUS,           // 5
    MINUS,          // 6
    MULTIPLY,       // 7
    DIVIDE,         // 8
    MODULUS,        // 9
    POWER,          // 10
    SEMICOLON,      // 11
    COMMA,          // 12
    LEFT_PAREN,     // 13
    RIGHT_PAREN,    // 14
    LEFT_BRACE,     // 15
    RIGHT_BRACE,    // 16
    // Keywords
    CLASS,          // 17
    FUNCTION,       // 18
    RETURN,         // 19
    // Add other tokens as needed

    IF,
    ELSE,
    EQUALS,        // ==
    NOT_EQUALS,    // !=
    LESS_THAN,     // <
    GREATER_THAN,  // >
    LESS_EQUAL,    // <=
    GREATER_EQUAL, // >=
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value) : type(type), value(value) {}
};

#endif // TOKEN_H
