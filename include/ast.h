#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include "token.h"

// Forward declarations
class AST;
using ASTPtr = std::unique_ptr<AST>;

class AST {
public:
    virtual ~AST() = default;
};

class BinOp : public AST {
public:
    ASTPtr left;
    Token op;
    ASTPtr right;

    BinOp(ASTPtr left, Token op, ASTPtr right);
};

class Num : public AST {
public:
    Token token;
    double value;

    Num(Token token);
};

class UnaryOp : public AST {
public:
    Token op;
    ASTPtr expr;

    UnaryOp(Token op, ASTPtr expr);
};

class Compound : public AST {
public:
    std::vector<ASTPtr> children;

    Compound() noexcept;
    void addChild(ASTPtr child);
};

class Assign : public AST {
public:
    ASTPtr left;  // Should be a Var node
    Token op;
    ASTPtr right; // Should be an expression node

    Assign(ASTPtr left, Token op, ASTPtr right);
};

class Var : public AST {
public:
    Token token;
    std::string value;

    Var(Token token);
};

class NoOp : public AST {
public:
    NoOp() noexcept;
};

// New AST Nodes
class FunctionDef : public AST {
public:
    std::string name;
    std::vector<std::string> params;
    ASTPtr body;

    FunctionDef(const std::string& name, const std::vector<std::string>& params, ASTPtr body);
};

class FunctionCall : public AST {
public:
    std::string name;
    std::vector<ASTPtr> args;

    FunctionCall(const std::string& name, std::vector<ASTPtr> args);
};

class ClassDef : public AST {
public:
    std::string name;
    std::vector<ASTPtr> methods;

    ClassDef(const std::string& name, std::vector<ASTPtr> methods);
};

class Return : public AST {
public:
    ASTPtr expr;

    Return(ASTPtr expr);
};

class IfStatement : public AST {
public:
    ASTPtr condition;
    ASTPtr thenBranch;
    ASTPtr elseBranch;

    IfStatement(ASTPtr condition, ASTPtr thenBranch, ASTPtr elseBranch = nullptr);
};


#endif // AST_H
