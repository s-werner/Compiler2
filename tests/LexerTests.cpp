#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/token.h"

// Helper function to tokenize an entire input string
std::vector<Token> tokenize(const std::string& input) {
    Lexer lexer(input);
    std::vector<Token> tokens;
    Token token = lexer.getNextToken();
    while (token.type != TokenType::END_OF_FILE) {
        tokens.push_back(token);
        token = lexer.getNextToken();
    }
    tokens.push_back(token); // Add END_OF_FILE
    return tokens;
}

TEST(LexerTest, HandlesIntegersAndFloats) {
    std::string input = "123 45.67 .89 0.001";
    std::vector<Token> tokens = tokenize(input);

    ASSERT_EQ(tokens.size(), 5); // 4 numbers + END_OF_FILE

    EXPECT_EQ(tokens[0].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[0].value, "123");

    EXPECT_EQ(tokens[1].type, TokenType::FLOAT);
    EXPECT_EQ(tokens[1].value, "45.67");

    EXPECT_EQ(tokens[2].type, TokenType::FLOAT);
    EXPECT_EQ(tokens[2].value, ".89");

    EXPECT_EQ(tokens[3].type, TokenType::FLOAT);
    EXPECT_EQ(tokens[3].value, "0.001");

    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesIdentifiers) {
    std::string input = "var1 _var2 var_3";
    std::vector<Token> tokens = tokenize(input);

    ASSERT_EQ(tokens.size(), 4); // 3 identifiers + END_OF_FILE

    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].value, "var1");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].value, "_var2");

    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].value, "var_3");

    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesOperatorsAndPunctuation) {
    std::string input = "+ - * / % ^ = ; ( )";
    std::vector<Token> tokens = tokenize(input);

    ASSERT_EQ(tokens.size(), 11); // 10 operators/punctuation + END_OF_FILE

    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[0].value, "+");

    EXPECT_EQ(tokens[1].type, TokenType::MINUS);
    EXPECT_EQ(tokens[1].value, "-");

    EXPECT_EQ(tokens[2].type, TokenType::MULTIPLY);
    EXPECT_EQ(tokens[2].value, "*");

    EXPECT_EQ(tokens[3].type, TokenType::DIVIDE);
    EXPECT_EQ(tokens[3].value, "/");

    EXPECT_EQ(tokens[4].type, TokenType::MODULUS);
    EXPECT_EQ(tokens[4].value, "%");

    EXPECT_EQ(tokens[5].type, TokenType::POWER);
    EXPECT_EQ(tokens[5].value, "^");

    EXPECT_EQ(tokens[6].type, TokenType::ASSIGN);
    EXPECT_EQ(tokens[6].value, "=");

    EXPECT_EQ(tokens[7].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[7].value, ";");

    EXPECT_EQ(tokens[8].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[8].value, "(");

    EXPECT_EQ(tokens[9].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(tokens[9].value, ")");

    EXPECT_EQ(tokens[10].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesWhitespace) {
    std::string input = " \t\n 123 \t\n 45.67 \n\t";
    std::vector<Token> tokens = tokenize(input);

    ASSERT_EQ(tokens.size(), 3); // 2 numbers + END_OF_FILE

    EXPECT_EQ(tokens[0].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[0].value, "123");

    EXPECT_EQ(tokens[1].type, TokenType::FLOAT);
    EXPECT_EQ(tokens[1].value, "45.67");

    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

TEST(LexerTest, HandlesInvalidCharacters) {
    std::string input = "@ # $";
    std::vector<Token> tokens = tokenize(input);

    ASSERT_EQ(tokens.size(), 4); // 3 UNKNOWN + END_OF_FILE

    EXPECT_EQ(tokens[0].type, TokenType::UNKNOWN);
    EXPECT_EQ(tokens[0].value, "@");

    EXPECT_EQ(tokens[1].type, TokenType::UNKNOWN);
    EXPECT_EQ(tokens[1].value, "#");

    EXPECT_EQ(tokens[2].type, TokenType::UNKNOWN);
    EXPECT_EQ(tokens[2].value, "$");

    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}
