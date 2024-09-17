#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "symboltable.h"
#include <unordered_map>
#include <string>

// Custom exception for return statements
class ReturnException : public std::exception {
public:
    double value;
    ReturnException(double val) : value(val) {}
};

class Interpreter {
public:
    Interpreter();
    double interpret(ASTPtr& tree);

    double getVariableValue(const std::string& name) const;

private:
    SymbolTable symbolTable;
    std::unordered_map<std::string, FunctionDef*> functions;
    std::unordered_map<std::string, ClassDef*> classes;

    int recursionDepth;
    const int MAX_RECURSION_DEPTH = 1000;

    // Visit methods
    double visit(AST* node);
    double visitBinOp(BinOp* node);
    double visitNum(Num* node);
    double visitUnaryOp(UnaryOp* node);
    double visitAssign(Assign* node);
    double visitVar(Var* node);
    double visitNoOp(NoOp* node);
    double visitCompound(Compound* node);
    double visitFunctionDef(FunctionDef* node);
    double visitFunctionCall(FunctionCall* node);
    double visitClassDef(ClassDef* node);
    double visitReturn(Return* node);
    double visitIfStatement(IfStatement* node);
};

#endif // INTERPRETER_H
