#include "symboltable.h"

SymbolTable::SymbolTable() {
    // Initialize with a global scope
    scopes.emplace_back();
}

void SymbolTable::set(const std::string& name, double value) {
    scopes.back()[name] = value;
}

double SymbolTable::get(const std::string& name) const {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto varIt = it->find(name);
        if (varIt != it->end()) {
            return varIt->second;
        }
    }
    throw std::runtime_error("Undefined variable: " + name);
}

void SymbolTable::enterScope() {
    scopes.emplace_back();
}

void SymbolTable::leaveScope() {
    if (scopes.size() > 1) {
        scopes.pop_back();
    } else {
        throw std::runtime_error("Cannot leave global scope");
    }
}
