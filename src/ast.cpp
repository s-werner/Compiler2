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

// FunctionDef Implementation
FunctionDef::FunctionDef(const std::string& name, const std::vector<std::string>& params, ASTPtr body)
    : name(name), params(params), body(std::move(body)) {}

// FunctionCall Implementation
FunctionCall::FunctionCall(const std::string& name, std::vector<ASTPtr> args)
    : name(name), args(std::move(args)) {}

// ClassDef Implementation
ClassDef::ClassDef(const std::string& name, std::vector<ASTPtr> methods)
    : name(name), methods(std::move(methods)) {}

// Return Implementation
Return::Return(ASTPtr expr) : expr(std::move(expr)) {}

IfStatement::IfStatement(ASTPtr condition, ASTPtr thenBranch, ASTPtr elseBranch)
    : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

