#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "symboltable.h"

class Interpreter {
public:
    Interpreter();
    double interpret(ASTPtr& tree);
private:
    double visit(AST* node);
    double visitBinOp(BinOp* node);
    double visitNum(Num* node);
    double visitUnaryOp(UnaryOp* node);
    double visitAssign(Assign* node);
    double visitVar(Var* node);
    double visitNoOp(NoOp* node);
    double visitCompound(Compound* node);
    SymbolTable symbolTable;
};

#endif // INTERPRETER_H
