#include "../include/Recorder.hpp"

#include <algorithm>
#include <iostream>
Recorder::~Recorder(){
    clear();
}
void Recorder::add(int line, Statement* stmt){
    //如果该行已存在，先删除原有语句
    if (hasLine(line)) remove(line);
    statements_[line] = stmt;
}
void Recorder::remove(int line){
    auto it = statements_.find(line);
    if (it != statements_.end()){
        delete it->second; //释放语句内存
        statements_.erase(it);
    }
}
const Statement* Recorder::get(int line) const noexcept{
    auto it = statements_.find(line);
    if (it != statements_.end()) return it->second;
    return nullptr;
}
bool Recorder::hasLine(int line) const noexcept{
    return statements_.find(line) != statements_.end();
}
void Recorder::clear() noexcept{
    for (auto& pair : statements_){
        delete pair.second; //释放所有语句内存
    }
    statements_.clear();
}
void Recorder::printLines() const{
    for (const auto& pair : statements_){
        std::cout << pair.first << ' ' << pair.second->text() << std::endl;
    }
}
int Recorder::nextLine(int line) const noexcept{
    if (statements_.empty()) return -1;
    auto it = statements_.upper_bound(line);
    if (it == statements_.end()) return -1; //没有下一行了
    return it->first;
}
// TODO: Imply interfaces declared in the Recorder.hpp.
// Created by Lenovo on 2025/11/19.
//
