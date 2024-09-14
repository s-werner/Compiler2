#include "ast.h"
#include <utility> // For std::move

// Binary Operation Node Constructor
BinOp::BinOp(ASTPtr left, Token op, ASTPtr right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

// Number Node Constructor
Num::Num(Token token)
    : token(token), value(std::stod(token.value)) {}

// Unary Operation Node Constructor
UnaryOp::UnaryOp(Token op, ASTPtr expr)
    : op(op), expr(std::move(expr)) {}

// Compound Node Constructor
Compound::Compound() noexcept {}

void Compound::addChild(ASTPtr child) {
    children.push_back(std::move(child));
}


// Assignment Node Constructor
Assign::Assign(ASTPtr left, Token op, ASTPtr right)
    : left(std::move(left)), op(op), right(std::move(right)) {}

// Variable Node Constructor
Var::Var(Token token)
    : token(token), value(token.value) {}

// No Operation Node Constructor
NoOp::NoOp() noexcept {}
