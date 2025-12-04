#include "../include/VarState.hpp"

#include <algorithm>

#include "../include/Error.hpp"

VarState::VarState() {
    // 初始化全局作用域
    scopes_.push(std::unordered_map<std::string, int>());
}

void VarState::setValue(const std::string& name, int value) {
    // 在当前作用域设置变量值
    scopes_.top()[name] = value;
}

int VarState::getValue(const std::string& name) const {
    // 从当前作用域开始查找变量
    std::stack<std::unordered_map<std::string, int>> temp = scopes_;
    while (!temp.empty()) {
        auto it = temp.top().find(name);
        if (it != temp.top().end()) {
            return it->second;
        }
        temp.pop();
    }
    throw BasicError("VARIABLE NOT DEFINED");
}

void VarState::clear() {
    // 清空所有作用域
    while (!scopes_.empty()) {
        scopes_.pop();
    }
    scopes_.push(std::unordered_map<std::string, int>());
}

void VarState::enterScope() {
    scopes_.push(std::unordered_map<std::string, int>());
}

void VarState::exitScope() {
    checkScopeUnderflow();
    if (scopes_.size() > 1) {
        scopes_.pop();
    }
}

int VarState::getScopeDepth() const {
    return scopes_.size() - 1;  // 深度从0开始
}

void VarState::checkScopeUnderflow() const {
    if (scopes_.size() <= 1) {
        throw BasicError("SCOPE UNDERFLOW");
    }
}
// Created by Lenovo on 2025/11/19.
//
