#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer) {
    currentToken = lexer.getNextToken();
    nextToken = lexer.getNextToken();
}

void Parser::eat(TokenType type) {
    if (currentToken.type == type) {
        currentToken = nextToken;
        nextToken = lexer.getNextToken();
    } else {
        throw std::runtime_error("Syntax error: Unexpected token '" + currentToken.value + "'");
    }
}

ASTPtr Parser::factor() {
    Token token = currentToken;
    if (token.type == TokenType::PLUS) {
        eat(TokenType::PLUS);
        return std::make_unique<UnaryOp>(token, factor());
    } else if (token.type == TokenType::MINUS) {
        eat(TokenType::MINUS);
        return std::make_unique<UnaryOp>(token, factor());
    } else if (token.type == TokenType::INTEGER || token.type == TokenType::FLOAT) {
        eat(token.type);
        return std::make_unique<Num>(token);
    } else if (token.type == TokenType::IDENTIFIER) {
        return variable();
    } else if (token.type == TokenType::LEFT_PAREN) {
        eat(TokenType::LEFT_PAREN);
        ASTPtr node = expr();
        eat(TokenType::RIGHT_PAREN);
        return node;
    } else {
        throw std::runtime_error("Syntax error: Invalid factor");
    }
}

ASTPtr Parser::term() {
    ASTPtr node = factor();
    while (currentToken.type == TokenType::MULTIPLY ||
           currentToken.type == TokenType::DIVIDE ||
           currentToken.type == TokenType::MODULUS ||
           currentToken.type == TokenType::POWER) {
        Token token = currentToken;
        if (token.type == TokenType::MULTIPLY) {
            eat(TokenType::MULTIPLY);
        } else if (token.type == TokenType::DIVIDE) {
            eat(TokenType::DIVIDE);
        } else if (token.type == TokenType::MODULUS) {
            eat(TokenType::MODULUS);
        } else if (token.type == TokenType::POWER) {
            eat(TokenType::POWER);
        }
        node = std::make_unique<BinOp>(std::move(node), token, factor());
    }
    return node;
}

ASTPtr Parser::expr() {
    ASTPtr node = term();
    while (currentToken.type == TokenType::PLUS ||
           currentToken.type == TokenType::MINUS) {
        Token token = currentToken;
        if (token.type == TokenType::PLUS) {
            eat(TokenType::PLUS);
        } else if (token.type == TokenType::MINUS) {
            eat(TokenType::MINUS);
        }
        node = std::make_unique<BinOp>(std::move(node), token, term());
    }
    return node;
}

ASTPtr Parser::variable() {
    Token token = currentToken;
    eat(TokenType::IDENTIFIER);
    return std::make_unique<Var>(token);
}

ASTPtr Parser::assignmentStatement() {
    ASTPtr left = variable();
    Token token = currentToken;
    eat(TokenType::ASSIGN);
    ASTPtr right = expr();
    return std::make_unique<Assign>(std::move(left), token, std::move(right));
}

ASTPtr Parser::statement() {
    ASTPtr node;
    if (currentToken.type == TokenType::IDENTIFIER && nextToken.type == TokenType::ASSIGN) {
        node = assignmentStatement();
    } else {
        node = expr();
    }
    if (currentToken.type == TokenType::SEMICOLON) {
        eat(TokenType::SEMICOLON);
    }
    return node;
}

ASTPtr Parser::program() {
    auto compound = std::make_unique<Compound>();
    while (currentToken.type != TokenType::END_OF_FILE) {
        compound->addChild(statement());
    }
    return compound;
}

ASTPtr Parser::parse() {
    ASTPtr node = program();
    if (currentToken.type != TokenType::END_OF_FILE) {
        throw std::runtime_error("Syntax error: Unexpected token at the end of input");
    }
    return node;
}
