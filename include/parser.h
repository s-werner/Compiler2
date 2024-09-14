#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
public:
    Parser(Lexer& lexer);
    ASTPtr parse();

private:
    Lexer& lexer;
    Token currentToken;
    Token nextToken;  // Lookahead token

    void eat(TokenType type);
    ASTPtr factor();
    ASTPtr term();
    ASTPtr expr();
    ASTPtr statement();
    ASTPtr assignmentStatement();
    ASTPtr variable();
    ASTPtr program();
};

#endif // PARSER_H
