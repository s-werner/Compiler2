#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()), nextToken(lexer.getNextToken()) {}

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
    if (token.type == TokenType::IDENTIFIER) {
        eat(TokenType::IDENTIFIER);
        if (currentToken.type == TokenType::LEFT_PAREN) {
            // Function call
            eat(TokenType::LEFT_PAREN);
            std::vector<ASTPtr> args;
            if (currentToken.type != TokenType::RIGHT_PAREN) {
                args.push_back(expr());
                while (currentToken.type == TokenType::COMMA) {
                    eat(TokenType::COMMA);
                    args.push_back(expr());
                }
            }
            eat(TokenType::RIGHT_PAREN);
            return std::make_unique<FunctionCall>(token.value, std::move(args));
        } else {
            // Variable
            return std::make_unique<Var>(token);
        }
    } else {
        if (token.type == TokenType::PLUS) {
            eat(TokenType::PLUS);
            return std::make_unique<UnaryOp>(token, factor());
        } else if (token.type == TokenType::MINUS) {
            eat(TokenType::MINUS);
            return std::make_unique<UnaryOp>(token, factor());
        } else if (token.type == TokenType::INTEGER || token.type == TokenType::FLOAT) {
            eat(token.type);
            return std::make_unique<Num>(token);
        } else if (token.type == TokenType::LEFT_PAREN) {
            eat(TokenType::LEFT_PAREN);
            ASTPtr node = expr();
            eat(TokenType::RIGHT_PAREN);
            return node;
        } else {
            throw std::runtime_error("Syntax error: Invalid factor");
        }
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
    if (currentToken.type == TokenType::IF) {
        return ifStatement();
    }  else if (currentToken.type == TokenType::CLASS) {
        return classDeclaration();
    } else if (currentToken.type == TokenType::FUNCTION) {
        return functionDeclaration();
    } else if (currentToken.type == TokenType::RETURN) {
        return returnStatement();
    } else if (currentToken.type == TokenType::IDENTIFIER && nextToken.type == TokenType::ASSIGN) {
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

ASTPtr Parser::classDeclaration() {
    eat(TokenType::CLASS);
    Token className = currentToken;
    eat(TokenType::IDENTIFIER);
    eat(TokenType::LEFT_BRACE);

    std::vector<ASTPtr> methods;

    while (currentToken.type != TokenType::RIGHT_BRACE) {
        methods.push_back(functionDeclaration());
    }

    eat(TokenType::RIGHT_BRACE);

    return std::make_unique<ClassDef>(className.value, std::move(methods));
}

ASTPtr Parser::functionDeclaration() {
    eat(TokenType::FUNCTION);
    Token funcName = currentToken;
    eat(TokenType::IDENTIFIER);
    eat(TokenType::LEFT_PAREN);

    std::vector<std::string> params;
    if (currentToken.type != TokenType::RIGHT_PAREN) {
        params.push_back(currentToken.value);
        eat(TokenType::IDENTIFIER);

        while (currentToken.type == TokenType::COMMA) {
            eat(TokenType::COMMA);
            params.push_back(currentToken.value);
            eat(TokenType::IDENTIFIER);
        }
    }
    eat(TokenType::RIGHT_PAREN);

    ASTPtr body = block();

    return std::make_unique<FunctionDef>(funcName.value, params, std::move(body));
}

ASTPtr Parser::block() {
    eat(TokenType::LEFT_BRACE);

    auto compound = std::make_unique<Compound>();
    while (currentToken.type != TokenType::RIGHT_BRACE) {
        compound->addChild(statement());
    }

    eat(TokenType::RIGHT_BRACE);
    return compound;
}

ASTPtr Parser::returnStatement() {
    eat(TokenType::RETURN);
    ASTPtr node = expr();
    if (currentToken.type == TokenType::SEMICOLON) {
        eat(TokenType::SEMICOLON);
    }
    return std::make_unique<Return>(std::move(node));
}

ASTPtr Parser::condition() {
    ASTPtr left = expr();
    Token op = currentToken;
    if (op.type == TokenType::EQUALS || op.type == TokenType::NOT_EQUALS ||
        op.type == TokenType::LESS_THAN || op.type == TokenType::GREATER_THAN ||
        op.type == TokenType::LESS_EQUAL || op.type == TokenType::GREATER_EQUAL) {
        eat(op.type);
        ASTPtr right = expr();
        return std::make_unique<BinOp>(std::move(left), op, std::move(right));
    } else {
        throw std::runtime_error("Invalid comparison operator");
    }
}

ASTPtr Parser::ifStatement() {
    eat(TokenType::IF);
    eat(TokenType::LEFT_PAREN);
    ASTPtr conditionNode = condition();
    eat(TokenType::RIGHT_PAREN);
    ASTPtr thenBranch = block();

    ASTPtr elseBranch = nullptr;
    if (currentToken.type == TokenType::ELSE) {
        eat(TokenType::ELSE);
        elseBranch = block();
    }

    return std::make_unique<IfStatement>(std::move(conditionNode), std::move(thenBranch), std::move(elseBranch));
}
