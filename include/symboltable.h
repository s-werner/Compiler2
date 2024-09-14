#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <unordered_map>
#include <string>

class SymbolTable {
public:
    void set(const std::string& name, double value);
    double get(const std::string& name);
private:
    std::unordered_map<std::string, double> table;
};

#endif // SYMBOLTABLE_H
