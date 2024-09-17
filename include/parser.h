#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser(Lexer& lexer);
    ASTPtr parse(); // Parses the entire input as a program (compound statements)

private:
    Lexer& lexer;
    Token currentToken;
    Token nextToken; // Lookahead token

    void eat(TokenType type);
    ASTPtr factor();
    ASTPtr term();
    ASTPtr expr();
    ASTPtr statement();
    ASTPtr assignmentStatement();
    ASTPtr variable();
    ASTPtr program();  // Method for parsing multiple statements
    ASTPtr classDeclaration();
    ASTPtr functionDeclaration();
    ASTPtr block();
    ASTPtr returnStatement();
    ASTPtr expressionList();
    ASTPtr condition();
    ASTPtr ifStatement();
};

#endif // PARSER_H
