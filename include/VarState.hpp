#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <stack>

class VarState {
public:
    VarState();
    void setValue(const std::string& name, int value);
    int getValue(const std::string& name) const;
    void clear();
    void enterScope();
    void exitScope();
    int getScopeDepth() const;
    void checkScopeUnderflow() const;

private:
    std::stack<std::unordered_map<std::string, int>> scopes_;
    bool findVariableInScopes(const std::string& name, int& value) const;
};
// Created by Lenovo on 2025/11/19.
//
