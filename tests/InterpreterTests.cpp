#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include <memory>
#include "TestUtils.h"

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

TEST(InterpreterTest, EvaluatesModulusOperator) {
    std::string input = "result = 10 % 3;";
    double result = interpretInput(input);
    EXPECT_DOUBLE_EQ(result, 1.0);
}

TEST(InterpreterTest, EvaluatesMultipleAssignments) {
    Interpreter interpreter;
    double result = interpretInput("x = 10; y = x + 5; z = y * 2; z;", interpreter);
    EXPECT_EQ(result, 30);
}

TEST(InterpreterTest, HandlesDivisionByZero) {
    std::string input = "result = 10 / 0;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    EXPECT_THROW(interpreter.interpret(tree), std::runtime_error);
}

TEST(InterpreterTest, HandlesUndefinedVariables) {
    std::string input = "result = a + 5;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    EXPECT_THROW(interpreter.interpret(tree), std::runtime_error);
}
/*
TEST(InterpreterTest, HandlesFunctionRedefinition) {
    std::string input = R"(
        function test() {
            return 1;
        }
        function test() {
            return 2;
        }
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    EXPECT_THROW(interpreter.interpret(tree), std::runtime_error);
}
*/

TEST(FunctionTest, HandlesMutualRecursion) {
    std::string input = R"(
        function isEven(n) {
            if (n == 0) {
                return 1;
            } else {
                return isOdd(n - 1);
            }
        }
        function isOdd(n) {
            if (n == 0) {
                return 0;
            } else {
                return isEven(n - 1);
            }
        }
        result = isEven(10);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), 1.0);
}

TEST(InterpreterTest, DetectsInfiniteRecursion) {
    std::string input = R"(
        function infinite() {
            return infinite();
        }
        result = infinite();
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    // Depending on your interpreter, this might cause a stack overflow
    // You may need to implement recursion depth limits

    EXPECT_THROW(interpreter.interpret(tree), std::runtime_error);
}

TEST(InterpreterTest, EvaluatesBasicArithmetic) {
    std::string input = R"(
        result1 = 1 + 2 - 3;
        result2 = 4 * 5 / 2;
        result3 = 7 % 4;
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result1"), 0.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), 10.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result3"), 3.0);
}

TEST(InterpreterTest, EvaluatesPowerOperator) {
    std::string input = "result = 2 ^ 3;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), 8.0);
}

/*
TEST(InterpreterTest, EvaluatesChainedComparisons) {
    std::string input = "result = 1 < 2 < 3;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;

    EXPECT_THROW({
        interpreter.interpret(tree);
    }, std::runtime_error);
}

TEST(InterpreterTest, HandlesMultipleAssignments) {
    std::string input = "a = b = c = 5;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("a"), 5.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("b"), 5.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("c"), 5.0);
}
*/

TEST(InterpreterTest, HandlesVariableScope) {
    std::string input = R"(
        a = 10;
        function testScope() {
            a = 5;
            return a;
        }
        result1 = testScope();
        result2 = a;
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result1"), 5.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), 10.0);  // If variables are local to functions
}

TEST(InterpreterTest, EvaluatesChainedFunctionCalls) {
    std::string input = R"(
        function increment(n) {
            return n + 1;
        }
        function double(n) {
            return n * 2;
        }
        result = double(increment(5));
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), 12.0);
}

/*
TEST(InterpreterTest, HandlesFunctionWithZeroAndNegativeInputs) {
    std::string input = R"(
        function reciprocal(n) {
            return 1 / n;
        }
        result1 = reciprocal(0);  // Should raise division by zero error
        result2 = reciprocal(-2);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    EXPECT_THROW(interpreter.interpret(tree), std::runtime_error);

    // Comment out the line that causes an error to test the negative input
    input = R"(
        function reciprocal(n) {
            return 1 / n;
        }
        result2 = reciprocal(-2);
    )";
    Lexer lexer2(input);
    Parser parser2(lexer);
    ASTPtr tree2 = parser2.parse();
    Interpreter interpreter2;
    interpreter2.interpret(tree2);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), -0.5);
}
*/

TEST(InterpreterTest, HandlesRecursiveFunctionEdgeCases) {
    std::string input = R"(
        function fibonacci(n) {
            if (n <= 1) {
                return n;
            } else {
                return fibonacci(n - 1) + fibonacci(n - 2);
            }
        }
        result = fibonacci(10);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), 55.0);
}

TEST(InterpreterTest, EvaluatesComplexExpressionsWithFunctions) {
    std::string input = R"(
        function complexCalc(a, b, c) {
            return (a ^ b) / (c - a) + (b % c) * a;
        }
        result = complexCalc(2, 3, 5);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    double expectedResult = (std::pow(2, 3) / (5 - 2)) + ((3 % 5) * 2);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), expectedResult);
}

TEST(InterpreterTest, EvaluatesNegativeExponents) {
    std::string input = "result = 2 ^ -2;";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_NEAR(interpreter.getVariableValue("result"), 0.25, 1e-6);
}

TEST(InterpreterTest, EvaluatesLargeNumbers) {
    std::string input = R"(
        result = 9999999999 * 8888888888;
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    double expectedResult = 9999999999.0 * 8888888888.0;
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result"), expectedResult);
}

TEST(InterpreterTest, HandlesFloatingPointPrecision) {
    std::string input = R"(
        result = 0.1 + 0.2;
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    // Due to floating point precision, the result might not be exactly 0.3
    EXPECT_NEAR(interpreter.getVariableValue("result"), 0.3, 1e-6);
}

/*
TEST(InterpreterTest, EvaluatesAssociativity) {
    std::string input = R"(
        result1 = 2 ^ 3 ^ 2;    // Should be 2 ^ (3 ^ 2)
        result2 = (2 ^ 3) ^ 2;  // Should be (2 ^ 3) ^ 2
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result1"), 512.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), 64.0);
}
*/

TEST(InterpreterTest, EvaluatesModulusWithNegativeNumbers) {
    std::string input = R"(
        result1 = -10 % 3;
        result2 = 10 % -3;
        result3 = -10 % -3;
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result1"), -1.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), 1.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result3"), -1.0);
}

TEST(InterpreterTest, EvaluatesPowerOperatorEdgeCases) {
    std::string input = R"(
        result1 = 2 ^ 0;
        result2 = 0 ^ 0;
        result3 = (-2) ^ 3;
        result4 = 2 ^ (-3);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    interpreter.interpret(tree);

    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result1"), 1.0);
    // Depending on your implementation, 0^0 might be undefined.
    // Here, let's assume it's 1
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result2"), 1.0);
    EXPECT_DOUBLE_EQ(interpreter.getVariableValue("result3"), -8.0);
    EXPECT_NEAR(interpreter.getVariableValue("result4"), 0.125, 1e-6);
}
