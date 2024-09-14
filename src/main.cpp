#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main() {
    std::string input;

    std::cout << "Welcome to MyCompiler! Type 'exit' to quit." << std::endl;

    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        // Create a Lexer with the input
        Lexer lexer(input);

        // Create a Parser with the Lexer
        Parser parser(lexer);

        try {
            // Parse the input to get the AST
            ASTPtr tree = parser.parse();

            // Create an Interpreter
            Interpreter interpreter;

            // Interpret the AST and get the result
            double result = interpreter.interpret(tree);

            // Print the result
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            // Handle any exceptions (syntax errors, runtime errors)
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
