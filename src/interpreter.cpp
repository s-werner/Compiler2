#include "interpreter.h"
#include <cmath>

Interpreter::Interpreter() {}

double Interpreter::interpret(ASTPtr& tree) {
    return visit(tree.get());
}

double Interpreter::visit(AST* node) {
    if (auto binOpNode = dynamic_cast<BinOp*>(node)) {
        return visitBinOp(binOpNode);
    } else if (auto numNode = dynamic_cast<Num*>(node)) {
        return visitNum(numNode);
    } else if (auto unaryOpNode = dynamic_cast<UnaryOp*>(node)) {
        return visitUnaryOp(unaryOpNode);
    } else if (auto assignNode = dynamic_cast<Assign*>(node)) {
        return visitAssign(assignNode);
    } else if (auto varNode = dynamic_cast<Var*>(node)) {
        return visitVar(varNode);
    } else if (auto noOpNode = dynamic_cast<NoOp*>(node)) {
        return visitNoOp(noOpNode);
    } else if (auto compoundNode = dynamic_cast<Compound*>(node)) {
        return visitCompound(compoundNode);
    } else {
        throw std::runtime_error("Unknown AST node");
    }
}

double Interpreter::visitBinOp(BinOp* node) {
    double left = visit(node->left.get());
    double right = visit(node->right.get());

    switch (node->op.type) {
        case TokenType::PLUS:
            return left + right;
        case TokenType::MINUS:
            return left - right;
        case TokenType::MULTIPLY:
            return left * right;
        case TokenType::DIVIDE:
            if (right == 0) {
                throw std::runtime_error("Division by zero");
            }
            return left / right;
        case TokenType::MODULUS:
            return std::fmod(left, right);
        case TokenType::POWER:
            return std::pow(left, right);
        default:
            throw std::runtime_error("Unknown operator");
    }
}

double Interpreter::visitNum(Num* node) {
    return node->value;
}

double Interpreter::visitUnaryOp(UnaryOp* node) {
    double value = visit(node->expr.get());
    if (node->op.type == TokenType::PLUS) {
        return +value;
    } else if (node->op.type == TokenType::MINUS) {
        return -value;
    } else {
        throw std::runtime_error("Unknown unary operator");
    }
}

double Interpreter::visitAssign(Assign* node) {
    std::string varName = dynamic_cast<Var*>(node->left.get())->value;
    double value = visit(node->right.get());
    symbolTable.set(varName, value);
    return value;
}

double Interpreter::visitVar(Var* node) {
    return symbolTable.get(node->value);
}

double Interpreter::visitNoOp(NoOp* node) {
    return 0;
}

double Interpreter::visitCompound(Compound* node) {
    double result = 0;
    for (auto& child : node->children) {
        result = visit(child.get());
    }
    return result;
}
