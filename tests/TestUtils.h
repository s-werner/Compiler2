#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/interpreter.h"
#include <memory>

ASTPtr parseInput(const std::string& input);
double interpretInput(const std::string& input);
double interpretInput(const std::string& input, Interpreter& interpreter);
std::vector<Token> tokenize(const std::string& input);

#endif // TEST_UTILS_H
