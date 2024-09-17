#include "TestUtils.h"

// Utility function to parse an input string and return the AST
ASTPtr parseInput(const std::string& input) {
    Lexer lexer(input);
    Parser parser(lexer);
    return parser.parse();
}

// Helper function to interpret an input string and return the result
double interpretInput(const std::string& input) {
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    Interpreter interpreter;
    return interpreter.interpret(tree);
}

// Helper function to interpret an input string and return the result
double interpretInput(const std::string& input, Interpreter& interpreter) {
    Lexer lexer(input);
    Parser parser(lexer);
    ASTPtr tree = parser.parse();
    return interpreter.interpret(tree);
}

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