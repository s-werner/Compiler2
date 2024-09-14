#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include <memory>

// Helper function to interpret an input string and return the result
double interpretInput(const std::string& input, Interpreter& interpreter) {
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    return interpreter.interpret(tree);
}

TEST(InterpreterTest, EvaluatesSimpleAddition) {
    Interpreter interpreter;
    double result = interpretInput("3 + 5;", interpreter);
    EXPECT_EQ(result, 8);
}

TEST(InterpreterTest, EvaluatesVariableAssignmentAndUsage) {
    Interpreter interpreter;
    interpretInput("a = 5;", interpreter);
    double result = interpretInput("a + 2;", interpreter);
    EXPECT_EQ(result, 7);
}

TEST(InterpreterTest, EvaluatesOperatorPrecedence) {
    Interpreter interpreter;
    double result = interpretInput("3 + 5 * 2;", interpreter);
    EXPECT_EQ(result, 13);
}

TEST(InterpreterTest, EvaluatesParentheses) {
    Interpreter interpreter;
    double result = interpretInput("(3 + 5) * 2;", interpreter);
    EXPECT_EQ(result, 16);
}

TEST(InterpreterTest, EvaluatesFloatingPointNumbers) {
    Interpreter interpreter;
    double result = interpretInput("3.5 + 2.5;", interpreter);
    EXPECT_DOUBLE_EQ(result, 6.0);
}

TEST(InterpreterTest, HandlesCompoundStatements) {
    Interpreter interpreter;
    double result = interpretInput("a = 5; b = a * 2; b + 3;", interpreter);
    EXPECT_EQ(result, 13);
}

TEST(InterpreterTest, HandlesUnaryOperations) {
    Interpreter interpreter;
    double result1 = interpretInput("-5 + 3;", interpreter);
    EXPECT_EQ(result1, -2);

    double result2 = interpretInput("+5 - 2;", interpreter);
    EXPECT_EQ(result2, 3);
}

TEST(InterpreterTest, ThrowsOnUndefinedVariable) {
    Interpreter interpreter;
    ASSERT_THROW(interpretInput("a + 2;", interpreter), std::runtime_error);
}

TEST(InterpreterTest, ThrowsOnDivisionByZero) {
    Interpreter interpreter;
    ASSERT_THROW(interpretInput("5 / 0;", interpreter), std::runtime_error);
}

TEST(InterpreterTest, EvaluatesPowerOperator) {
    Interpreter interpreter;
    double result = interpretInput("2 ^ 3;", interpreter);
    EXPECT_EQ(result, 8);
}

TEST(InterpreterTest, EvaluatesModulusOperator) {
    Interpreter interpreter;
    double result = interpretInput("10 % 3;", interpreter);
    EXPECT_DOUBLE_EQ(result, 1.0);
}

TEST(InterpreterTest, EvaluatesMultipleAssignments) {
    Interpreter interpreter;
    double result = interpretInput("x = 10; y = x + 5; z = y * 2; z;", interpreter);
    EXPECT_EQ(result, 30);
}
