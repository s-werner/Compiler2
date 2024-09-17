#include <iostream> // Add this for debugging

#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& text) : text(text), pos(0), currentChar(text[pos]) {
    keywords = {
        {"class", TokenType::CLASS},
        {"function", TokenType::FUNCTION},
        {"return", TokenType::RETURN},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
    };
}

void Lexer::advance() {
    pos++;
    if (pos > text.size() - 1) {
        currentChar = '\0';  // Indicates end of input
    } else {
        currentChar = text[pos];
    }
}

void Lexer::skipWhitespace() {
    while (currentChar != '\0' && std::isspace(currentChar)) {
        advance();
    }
}

Token Lexer::integer() {
    std::string result;
    while (currentChar != '\0' && std::isdigit(currentChar)) {
        result += currentChar;
        advance();
    }
    return Token(TokenType::INTEGER, result);
}

Token Lexer::identifier() {
    std::string result;
    while (currentChar != '\0' && (std::isalnum(currentChar) || currentChar == '_')) {
        result += currentChar;
        advance();
    }
    // Check if the identifier is a reserved keyword
    auto keywordIt = keywords.find(result);
    if (keywordIt != keywords.end()) {
        // It's a keyword
        return Token(keywordIt->second, result);
    } else {
        // It's an identifier
        return Token(TokenType::IDENTIFIER, result);
    }
}


Token Lexer::number() {
    std::string result;
    while (currentChar != '\0' && std::isdigit(currentChar)) {
        result += currentChar;
        advance();
    }

    if (currentChar == '.') {
        result += currentChar;
        advance();

        while (currentChar != '\0' && std::isdigit(currentChar)) {
            result += currentChar;
            advance();
        }

        return Token(TokenType::FLOAT, result);
    }

    return Token(TokenType::INTEGER, result);
}

Token Lexer::getNextToken() {
    while (currentChar != '\0') {
        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        if (std::isalpha(currentChar) || currentChar == '_') {
            return identifier();
        }

        if (std::isdigit(currentChar)) {
            return number();
        }

        // Symbols
        switch (currentChar) {
            case '=':
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token(TokenType::EQUALS, "==");
                } else {
                    return Token(TokenType::ASSIGN, "=");
                }
            case '!':
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token(TokenType::NOT_EQUALS, "!=");
                } else {
                    throw std::runtime_error("Invalid token '!' without '='");
                }
            case '<':
                advance();
                if(currentChar == '=') {
                    advance();
                    return Token(TokenType::LESS_EQUAL, "<=");
                } else {
                    return Token(TokenType::LESS_THAN, "<");
                }
            case '>':
                advance();
                if (currentChar == '=') {
                    advance();
                    return Token(TokenType::GREATER_EQUAL, ">=");
                } else {
                    return Token(TokenType::GREATER_THAN, ">");
                }
            case '+':
                advance();
                return Token(TokenType::PLUS, "+");
            case '-':
                advance();
                return Token(TokenType::MINUS, "-");
            case '*':
                advance();
                return Token(TokenType::MULTIPLY, "*");
            case '/':
                advance();
                return Token(TokenType::DIVIDE, "/");
            case ';':
                advance();
                return Token(TokenType::SEMICOLON, ";");
            case ',':
                advance();
                return Token(TokenType::COMMA, ",");
            case '(':
                advance();
                return Token(TokenType::LEFT_PAREN, "(");
            case ')':
                advance();
                return Token(TokenType::RIGHT_PAREN, ")");
            case '{':
                advance();
                return Token(TokenType::LEFT_BRACE, "{");
            case '}':
                advance();
                return Token(TokenType::RIGHT_BRACE, "}");
            case '^':
                advance();
                return Token(TokenType::POWER, "^");
            case '%':
                advance();
                return Token(TokenType::MODULUS, "%");
            default:
                throw std::runtime_error("Syntax error: Invalid factor");
        }
        throw std::runtime_error(std::string("Invalid character: ") + currentChar);
    }

    // Return END_OF_FILE token when no more characters are left to process
    return Token(TokenType::END_OF_FILE, "");
}

