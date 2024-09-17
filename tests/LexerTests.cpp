#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/token.h"

TEST(LexerTest, RecognizesKeywords) {
    std::string input = "class function return";
    Lexer lexer(input);

    Token t1 = lexer.getNextToken();
    EXPECT_EQ(t1.type, TokenType::CLASS) << "Expected 'class' to be TokenType::CLASS";
    EXPECT_EQ(t1.value, "class");

    Token t2 = lexer.getNextToken();
    EXPECT_EQ(t2.type, TokenType::FUNCTION) << "Expected 'function' to be TokenType::FUNCTION";
    EXPECT_EQ(t2.value, "function");

    Token t3 = lexer.getNextToken();
    EXPECT_EQ(t3.type, TokenType::RETURN) << "Expected 'return' to be TokenType::RETURN";
    EXPECT_EQ(t3.value, "return");

    Token t4 = lexer.getNextToken();
    EXPECT_EQ(t4.type, TokenType::END_OF_FILE);
}
