#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

TEST(LexerTest, HandlesNumbers) {
    Lexer lexer("123 45.67");
    Token token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::INTEGER);
    EXPECT_EQ(token.value, "123");

    token = lexer.getNextToken();
    EXPECT_EQ(token.type, TokenType::FLOAT);
    EXPECT_EQ(token.value, "45.67");
}

TEST(ParserTest, ParsesExpressions) {
    Lexer lexer("3 + 5 * (10 - 20)");
    Parser parser(lexer);
    ASSERT_NO_THROW({
        ASTPtr tree = parser.parse();
        ASSERT_NE(tree, nullptr);
    });
}

TEST(InterpreterTest, EvaluatesExpressions) {
    Lexer lexer("a = 5; b = a * 2");
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    double result = interpreter.interpret(tree);
    EXPECT_EQ(result, 10);

    Lexer lexer2("a = 5; b = a * 2; b;");
    Parser parser2(lexer2);
    ASTPtr tree2 = parser2.parse();
    Interpreter interpreter2;
    double result2 = interpreter2.interpret(tree2);
    EXPECT_EQ(result2, 10);
}
