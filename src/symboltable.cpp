#include "symboltable.h"
#include <stdexcept>

void SymbolTable::set(const std::string& name, double value) {
    table[name] = value;
}

double SymbolTable::get(const std::string& name) {
    if (table.find(name) != table.end()) {
        return table[name];
    } else {
        throw std::runtime_error("Variable '" + name + "' is not defined");
    }
}
