#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
public:
    explicit Statement(std::string source);
    virtual ~Statement() = default;

    virtual void execute(VarState& state, Program& program) const = 0;

    const std::string& text() const noexcept;

private:
    std::string source_;
};

class LetStmt : public Statement{
public:
    LetStmt(const std::string& varName, Expression* expr, const std::string& source);
    ~LetStmt() override;
    void execute(VarState& state, Program& program) const override;

private:
    std::string varName_;
    Expression* expr;
};

class PrintStmt : public Statement {
public:
    PrintStmt(Expression* expr, const std::string& source);
    ~PrintStmt() override;
    void execute(VarState& state, Program& program) const override;

private:
    Expression* expr_;
};

class InputStmt : public Statement {
public:
    InputStmt(const std::string& varName, const std::string& source);
    void execute(VarState& state, Program& program) const override;

private:
    std::string varName;
};

class GotoStmt : public Statement {
public:
    GotoStmt(int targetLine, const std::string& source);
    void execute(VarState& state, Program& program) const override;

private:
    int targetLine_;
};

class IfStmt : public Statement {
public:
    IfStmt(Expression* leftExpr, char op, Expression* rightExpr, int targetLine, const std::string& source);
    ~IfStmt() override;
    void execute(VarState& state, Program& program) const override;

private:
    Expression* leftExpr_;
    Expression* rightExpr_;
    char op_;
    int targetLine_;
};

class RemStmt : public Statement {
public:
    RemStmt(const std::string& remark, const std::string& source);
    void execute(VarState& state, Program& program) const override;

private:
    std::string remark;
};

class EndStmt : public Statement {
public:
    EndStmt(const std::string& source);
    void execute(VarState& state, Program& program) const override;
};

class IndentStmt : public Statement {
public:
    IndentStmt(const std::string& source);
    void execute(VarState& state, Program& program) const override;
};

class DedentStmt : public Statement {
public:
    DedentStmt(const std::string& source);
    void execute(VarState& state, Program& program) const override;
};
// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
// Created by Lenovo on 2025/11/19.
//
