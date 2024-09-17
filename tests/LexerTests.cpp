#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/token.h"

TEST(LexerTest, RecognizesAllTokens) {
    std::string input = R"(
        class function return if else
        + - * / % ^ == != < > <= >= = ; , ( ) { }
        123 45.67 _identifier identifier_123
    )";

    Lexer lexer(input);

    std::vector<TokenType> expectedTokens = {
        TokenType::CLASS, TokenType::FUNCTION, TokenType::RETURN,
        TokenType::IF, TokenType::ELSE,
        TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY,
        TokenType::DIVIDE, TokenType::MODULUS, TokenType::POWER,
        TokenType::EQUALS, TokenType::NOT_EQUALS, TokenType::LESS_THAN,
        TokenType::GREATER_THAN, TokenType::LESS_EQUAL, TokenType::GREATER_EQUAL,
        TokenType::ASSIGN, TokenType::SEMICOLON, TokenType::COMMA,
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::INTEGER, TokenType::FLOAT,
        TokenType::IDENTIFIER, TokenType::IDENTIFIER,
        TokenType::END_OF_FILE
    };

    for (const auto& expectedType : expectedTokens) {
        Token token = lexer.getNextToken();
        EXPECT_EQ(token.type, expectedType) << "Expected token type " << static_cast<int>(expectedType)
                                            << " but got " << static_cast<int>(token.type)
                                            << " with value '" << token.value << "'";
    }
}

TEST(LexerTest, HandlesIdentifiers) {
    std::string input = "var _var var1 var_name _123var";
    Lexer lexer(input);

    std::vector<std::string> expectedValues = {"var", "_var", "var1", "var_name", "_123var"};

    for (const auto& expectedValue : expectedValues) {
        Token token = lexer.getNextToken();
        EXPECT_EQ(token.type, TokenType::IDENTIFIER);
        EXPECT_EQ(token.value, expectedValue);
    }
}

TEST(LexerTest, HandlesNumbers) {
    std::string input = "0 00 0.0 .5 5. .123456789 1234567890";
    Lexer lexer(input);

    std::vector<std::string> expectedValues = {"0", "00", "0.0", ".5", "5.", ".123456789", "1234567890"};
    std::vector<TokenType> expectedTypes = {
        TokenType::INTEGER, TokenType::INTEGER, TokenType::FLOAT,
        TokenType::FLOAT, TokenType::FLOAT, TokenType::FLOAT, TokenType::INTEGER
    };

    for (size_t i = 0; i < expectedValues.size(); ++i) {
        Token token = lexer.getNextToken();
        EXPECT_EQ(token.type, expectedTypes[i]);
        EXPECT_EQ(token.value, expectedValues[i]);
    }
}

TEST(LexerTest, HandlesOperatorsAndDelimiters) {
    std::string input = "+ - * / % ^ = == != < > <= >= ; , ( ) { }";
    Lexer lexer(input);

    std::vector<TokenType> expectedTokens = {
        TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY,
        TokenType::DIVIDE, TokenType::MODULUS, TokenType::POWER,
        TokenType::ASSIGN, TokenType::EQUALS, TokenType::NOT_EQUALS,
        TokenType::LESS_THAN, TokenType::GREATER_THAN, TokenType::LESS_EQUAL,
        TokenType::GREATER_EQUAL, TokenType::SEMICOLON, TokenType::COMMA,
        TokenType::LEFT_PAREN, TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE, TokenType::RIGHT_BRACE,
        TokenType::END_OF_FILE
    };

    for (const auto& expectedType : expectedTokens) {
        Token token = lexer.getNextToken();
        EXPECT_EQ(token.type, expectedType);
    }
}
