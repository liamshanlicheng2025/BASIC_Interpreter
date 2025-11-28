#include "../include/Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "../include/Error.hpp"
#include "../include/Program.hpp"
#include "../include/VarState.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }
LetStmt::LetStmt(const std::string& varName, Expression* expr, const std::string& source) : Statement(source), varName_(varName), expr(expr){}
LetStmt::~LetStmt(){
    delete expr;
}
void LetStmt::execute(VarState& state, Program& program) const{
    int value = expr->evaluate(state);
    state.setValue(varName_, value);
}

PrintStmt::PrintStmt(Expression* expr, const std::string& source) : Statement(source), expr_(expr){}
PrintStmt::~PrintStmt(){
    delete expr_;
}
void PrintStmt::execute(VarState& state, Program& program) const{
    int value = expr_->evaluate(state);
    std::cout << value << std::endl;
}

InputStmt::InputStmt(const std::string& varName, const std::string& source) : Statement(source), varName(varName){}
void InputStmt::execute(VarState& state, Program& program) const{
    std::string input;
    while (true)
    {
      std::cout << " ? ";
      std::getline(std::cin, input);
      try{
        size_t pos;
        int value = std::stoi(input, &pos);
        if (pos == input.size()){
          state.setValue(varName, value);
          break;
        }
        else{
          std::cout << "INVALID NUMBER\n";
        }
      } catch (const std::exception&){
        std::cout << "INVALID NUMBER\n";
      }
    }
}

GotoStmt::GotoStmt(int targetLine, const std::string& source) : Statement(source), targetLine_(targetLine){}
void GotoStmt::execute(VarState& state, Program& program) const{
    program.changePC(targetLine_);
}

IfStmt::IfStmt(Expression* leftExpr, char op, Expression* rightExpr, int targetLine, const std::string& source)
    : Statement(source), leftExpr_(leftExpr), op_(op), rightExpr_(rightExpr), targetLine_(targetLine){}
IfStmt::~IfStmt(){
    delete leftExpr_;
    delete rightExpr_;
}
void IfStmt::execute(VarState& state, Program& program) const{
    int leftValue = leftExpr_->evaluate(state);
    int rightValue = rightExpr_->evaluate(state);
    bool judge;
    switch (op_){
        case '=':
            judge = (leftValue == rightValue);
            break;
        case '>':
            judge = (leftValue > rightValue);
            break;
        case '<':
            judge = (leftValue < rightValue);
            break;
    }
    if (judge){
        program.changePC(targetLine_);
    }
}

RemStmt::RemStmt(const std::string& remark, const std::string& source) : Statement(source), remark(remark){}
void RemStmt::execute(VarState& state, Program& program) const {
    //不执行任何操作
}

EndStmt::EndStmt(const std::string& source) : Statement(source){}
void EndStmt::execute(VarState& state, Program& program) const {
    program.programEnd();
}
// TODO: Imply interfaces declared in the Statement.hpp.
// Created by Lenovo on 2025/11/19.
//
