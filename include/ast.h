#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"

class AST;
using ASTPtr = std::unique_ptr<AST>;

class AST {
public:
    virtual ~AST() = default;
};

// Binary Operation Node
class BinOp : public AST {
public:
    ASTPtr left;
    Token op;
    ASTPtr right;

    BinOp(ASTPtr left, Token op, ASTPtr right);
};

// Number Node
class Num : public AST {
public:
    Token token;
    double value;

    Num(Token token);
};

// Unary Operation Node
class UnaryOp : public AST {
public:
    Token op;
    ASTPtr expr;

    UnaryOp(Token op, ASTPtr expr);
};

// Compound Node (for holding a list of statements)
class Compound : public AST {
public:
    std::vector<ASTPtr> children;

    Compound() noexcept;
    void addChild(ASTPtr child);
};


// Assignment Node
class Assign : public AST {
public:
    ASTPtr left;
    Token op;
    ASTPtr right;

    Assign(ASTPtr left, Token op, ASTPtr right);
};

// Variable Node
class Var : public AST {
public:
    Token token;
    std::string value;

    Var(Token token);
};

// No Operation Node
class NoOp : public AST {
public:
    NoOp() noexcept;
};

#endif // AST_H
