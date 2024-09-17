#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <stdexcept>

class SymbolTable {
public:
    SymbolTable();

    void set(const std::string& name, double value);
    double get(const std::string& name) const;

    void enterScope();
    void leaveScope();

private:
    std::vector<std::unordered_map<std::string, double>> scopes;
};

#endif // SYMBOLTABLE_H
