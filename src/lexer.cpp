#include <iostream> // Add this for debugging

#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& text)
    : text(text), pos(0), currentChar(text[pos]) {}

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
    return {TokenType::INTEGER, result};
}

Token Lexer::identifier() {
    std::string result;
    while (currentChar != '\0' && (std::isalnum(currentChar) || currentChar == '_')) {
        result += currentChar;
        advance();
    }
    return {TokenType::IDENTIFIER, result};
}

Token Lexer::number() {
    std::string result;
    bool hasDecimalPoint = false;

    // Loop to build the number string, handling integers and floats
    while (currentChar != '\0' && (std::isdigit(currentChar) || currentChar == '.')) {
        if (currentChar == '.') {
            if (hasDecimalPoint) {
                // If there's already a decimal point, it should stop reading further
                break;  // This prevents multiple decimal points
            }
            hasDecimalPoint = true;
        }
        result += currentChar;
        advance();
    }

    // Check if the number has a decimal point to decide if it's a float or integer
    if (hasDecimalPoint) {
        return {TokenType::FLOAT, result};
    } else {
        return {TokenType::INTEGER, result};
    }
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
        if (std::isdigit(currentChar) || currentChar == '.') {
            return number();
        }
        if (currentChar == '+') {
            advance();
            return {TokenType::PLUS, "+"};
        }
        if (currentChar == '-') {
            advance();
            return {TokenType::MINUS, "-"};
        }
        if (currentChar == '*') {
            advance();
            return {TokenType::MULTIPLY, "*"};
        }
        if (currentChar == '/') {
            advance();
            return {TokenType::DIVIDE, "/"};
        }
        if (currentChar == '%') {
            advance();
            return {TokenType::MODULUS, "%"};
        }
        if (currentChar == '^') {
            advance();
            return {TokenType::POWER, "^"};
        }
        if (currentChar == '=') {
            advance();
            return {TokenType::ASSIGN, "="};
        }
        if (currentChar == ';') {
            advance();
            return {TokenType::SEMICOLON, ";"};
        }
        if (currentChar == '(') {
            advance();
            return {TokenType::LEFT_PAREN, "("};
        }
        if (currentChar == ')') {
            advance();
            return {TokenType::RIGHT_PAREN, ")"};
        }

        // If the character is not recognized, move forward and return unknown token
        std::string unknownChar(1, currentChar);
        Token token = {TokenType::UNKNOWN, std::string(1, currentChar)};
        std::cout << "Generated token: " << token.value << std::endl;
        advance();
        return {TokenType::UNKNOWN, unknownChar};
    }

    // Return END_OF_FILE token when no more characters are left to process
    return {TokenType::END_OF_FILE, ""};
}

