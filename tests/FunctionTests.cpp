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
