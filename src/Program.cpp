#include "../include/Program.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "../include/Error.hpp"
#include "../include/Statement.hpp"
Program::Program() : programCounter_(-1), programEnd_(false){
    //初始化programCounter_和programEnd
}
void Program::addStmt(int line, Statement* stmt){
    recorder_.add(line, stmt);
}
void Program::removeStmt(int line){
    recorder_.remove(line);
}
void Program::run(){
    try {
        programCounter_ = -1;
        programEnd_ = false;
        int first_line = findFirstLine();
        if (first_line == -1){
            return;
        }
        programCounter_ = first_line;
        while (!programEnd_ && programCounter_ != -1){
            //获取当前一行
            const Statement* stmt = recorder_.get(programCounter_);
            if (!stmt){
                programCounter_ = recorder_.nextLine(programCounter_);
                if (programCounter_ == -1) break;
                continue;
            }
            int currentPC = programCounter_;
            //执行当前语句
            stmt -> execute(vars_, *this);
            //跳转
            if (!programEnd_ && programCounter_ == currentPC){
                programCounter_ = recorder_.nextLine(programCounter_);
                if (programCounter_ == -1) break;
            }
        }
    } catch (const BasicError& e){
        std::cout << e.message() << std::endl;
    } catch (const std::exception& e){
        std::cout << "Unexpected error: "  << e.what() << std::endl;
    }
    resetAfterRun();
}
void Program::list() const{
    int line = findFirstLine();
    if (line == -1){
        return;
    }
    //遍历所有行
    while (line != -1){
        const Statement* stmt = recorder_.get(line);
        if (stmt){
            std::cout << stmt -> text() << std::endl;
        }
        line = recorder_.nextLine(line);
    }
}
void Program::clear(){
    // 清空 Recorder 与 VarState
    recorder_.clear();
    vars_.clear();
    resetAfterRun();
}
void Program::execute(Statement* stmt){
    // 执行一条语句
    try{
        stmt -> execute(vars_, *this);
    } catch (const BasicError&){
        throw;
    } catch (const std::exception& e){
        throw BasicError(std::string("Statement execution failed: ") + e.what());
    }
}
int Program::getPC() const noexcept{
    //获取当前行号
    return programCounter_;
}
void Program::changePC(int line){
    //检查目标行号是否存在
    if (!recorder_.hasLine(line)){
      throw BasicError("LINE NUMBER ERROR");
    }
    programCounter_ = line;
}
void Program::programEnd(){
    programEnd_ = true;
    programCounter_ = -1;
}
void Program::resetAfterRun() noexcept{
    programCounter_ = -1;
    programEnd_ = false;
}
int Program::findFirstLine() const{
    int line = 0;
    while (line != -1 && !recorder_.hasLine(line)){
        line = recorder_.nextLine(line);
    }
    return line;
}
// TODO: Imply interfaces declared in the Program.hpp.
// Created by Lenovo on 2025/11/19.
//
