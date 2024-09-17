#include "interpreter.h"
#include <cmath>
#include <stdexcept>

Interpreter::Interpreter() : recursionDepth(0) {}

double Interpreter::interpret(ASTPtr& tree) {
    return visit(tree.get());
}

double Interpreter::getVariableValue(const std::string& name) const {
    return symbolTable.get(name);
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
    } else if (auto funcDefNode = dynamic_cast<FunctionDef*>(node)) {
        return visitFunctionDef(funcDefNode);
    } else if (auto funcCallNode = dynamic_cast<FunctionCall*>(node)) {
        return visitFunctionCall(funcCallNode);
    } else if (auto classDefNode = dynamic_cast<ClassDef*>(node)) {
        return visitClassDef(classDefNode);
    } else if (auto returnNode = dynamic_cast<Return*>(node)) {
        return visitReturn(returnNode);
    } else if (auto ifNode = dynamic_cast<IfStatement*>(node)) {
        return visitIfStatement(ifNode);
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
        case TokenType::EQUALS:
            return left == right ? 1.0 : 0.0;
        case TokenType::NOT_EQUALS:
            return left != right ? 1.0 : 0.0;
        case TokenType::LESS_THAN:
            return left < right ? 1.0 : 0.0;
        case TokenType::GREATER_THAN:
            return left > right ? 1.0 : 0.0;
        case TokenType::LESS_EQUAL:
            return left <= right ? 1.0 : 0.0;
        case TokenType::GREATER_EQUAL:
            return left >= right ? 1.0 : 0.0;
        // ... other cases
        default:
            throw std::runtime_error("Unknown operator in binary operation");
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
    Var* varNode = dynamic_cast<Var*>(node->left.get());
    if (!varNode) {
        throw std::runtime_error("Left-hand side of assignment must be a variable");
    }
    std::string varName = varNode->value;
    double value = visit(node->right.get());
    symbolTable.set(varName, value);
    return value;
}

double Interpreter::visitVar(Var* node) {
    return symbolTable.get(node->value);
}

double Interpreter::visitNoOp(NoOp* node) {
    return 0.0;
}

double Interpreter::visitCompound(Compound* node) {
    double result = 0.0;
    for (auto& child : node->children) {
        result = visit(child.get());
    }
    return result;
}

double Interpreter::visitFunctionDef(FunctionDef* node) {
    // Store the function definition in the functions map
    functions[node->name] = node;
    return 0.0;
}

double Interpreter::visitFunctionCall(FunctionCall* node) {
    auto it = functions.find(node->name);
    if (it == functions.end()) {
        throw std::runtime_error("Undefined function: " + node->name);
    }

    FunctionDef* funcDef = it->second;

    // Check for maximum recursion depth
    recursionDepth++;
    if (recursionDepth > MAX_RECURSION_DEPTH) {
        recursionDepth--;
        throw std::runtime_error("Maximum recursion depth exceeded in function: " + node->name);
    }

    // Create a new symbol table for the function scope
    symbolTable.enterScope();

    // Check if the number of arguments matches
    if (node->args.size() != funcDef->params.size()) {
        recursionDepth--;
        throw std::runtime_error("Incorrect number of arguments in function call: " + node->name);
    }

    // Assign arguments to parameters
    for (size_t i = 0; i < node->args.size(); ++i) {
        double argValue = visit(node->args[i].get());
        symbolTable.set(funcDef->params[i], argValue);
    }

    double result = 0.0;

    // Execute the function body
    try {
        result = visit(funcDef->body.get());
    } catch (const ReturnException& ret) {
        result = ret.value;
    }

    // Clean up
    symbolTable.leaveScope();
    recursionDepth--;

    return result;
}

double Interpreter::visitClassDef(ClassDef* node) {
    // For simplicity, store class definitions similarly to functions
    classes[node->name] = node;
    return 0.0;
}

double Interpreter::visitReturn(Return* node) {
    double value = visit(node->expr.get());
    throw ReturnException(value);
}

double Interpreter::visitIfStatement(IfStatement* node) {
    double conditionValue = visit(node->condition.get());
    if (conditionValue != 0.0) {
        return visit(node->thenBranch.get());
    } else if (node->elseBranch) {
        return visit(node->elseBranch.get());
    }
    return 0.0;
}
