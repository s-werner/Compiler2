#include "ast.h"

// BinOp Implementation
BinOp::BinOp(ASTPtr left, Token op, ASTPtr right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

// Num Implementation
Num::Num(Token token) : token(token), value(std::stod(token.value)) {}

// UnaryOp Implementation
UnaryOp::UnaryOp(Token op, ASTPtr expr)
    : op(op), expr(std::move(expr)) {}

// Compound Implementation
Compound::Compound() noexcept {}

void Compound::addChild(ASTPtr child) {
    children.push_back(std::move(child));
}

// Assign Implementation
Assign::Assign(ASTPtr left, Token op, ASTPtr right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

// Var Implementation
Var::Var(Token token) : token(token), value(token.value) {}

// NoOp Implementation
NoOp::NoOp() noexcept {}
