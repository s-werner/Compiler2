#include <gtest/gtest.h>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"
#include "TestUtils.h"

TEST(FunctionTest, DefinesAndCallsFunction) {
    std::string input = R"(
        function add(a, b) {
            return a + b;
        }
        result = add(5, 3);
    )";
    double result = interpretInput(input);
    EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST(FunctionTest, HandlesRecursion) {
    std::string input = R"(
        function factorial(n) {
            if (n == 0) {
                return 1;
            } else {
                return n * factorial(n - 1);
            }
        }
        result = factorial(5);
    )";
    double result = interpretInput(input);
    EXPECT_DOUBLE_EQ(result, 120.0);
}

/*
TEST(FunctionTest, HandlesFunctionRedefinitionWithSameParameters) {
    std::string input = R"(
        function add(a, b) {
            return a + b;
        }
        function add(a, b) {
            return a - b;
        }
        result = add(5, 3);
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;

    EXPECT_THROW({
        interpreter.interpret(tree);
    }, std::runtime_error);

TEST(FunctionTest, HandlesNestedFunctionDefinitions) {
    std::string input = R"(
        function outer() {
            function inner() {
                return 42;
            }
            return inner();
        }
        result = outer();
    )";
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;

    // Depending on implementation, this may throw an error or work
    // If nested functions are not supported, it should throw an error
    EXPECT_THROW({
        interpreter.interpret(tree);
    }, std::runtime_error);
}

}*/