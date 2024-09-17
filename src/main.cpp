#include <iostream>
#include <fstream>
#include <sstream>
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

int main(int argc, char* argv[]) {
    std::string input;

    if (argc > 1) {
        // Read input from the file specified as the first command-line argument
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1] << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        input = buffer.str();
    } else {
        // Read input from standard input
        std::cout << "Enter code (end with EOF/Ctrl+D):" << std::endl;
        std::stringstream buffer;
        buffer << std::cin.rdbuf();
        input = buffer.str();
    }

    try {
        Lexer lexer(input);
        Parser parser(lexer);
        ASTPtr tree = parser.parse();

        Interpreter interpreter;
        interpreter.interpret(tree);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
