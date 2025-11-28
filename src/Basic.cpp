#include <cctype>
#include <iostream>
#include <memory>
#include <string>

#include "../include/Error.hpp"
#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"
#include "../include/Token.hpp"
bool isImmediateCommand(const std::string& line) {
  if (line.empty()) return false;

  // 跳过前导空格
  size_t start = 0;
  while (start < line.length() && std::isspace(line[start])) {
    start++;
  }

  // 检查第一个单词是否为命令
  size_t end = start;
  while (end < line.length() && std::isalpha(line[end])) {
    end++;
  }

  std::string firstWord = line.substr(start, end - start);

  return (firstWord == "RUN" || firstWord == "LIST" || firstWord == "CLEAR" ||
          firstWord == "QUIT" || firstWord == "HELP");
}

// 处理立即执行命令
bool handleImmediateCommand(const std::string& line, Program& program, bool& shouldQuit) {
  Lexer lexer;
  TokenStream tokens = lexer.tokenize(line);

  if (tokens.empty()) {
    return false;
  }

  const Token* firstToken = tokens.get();
  if (!firstToken) {
    return false;
  }
  switch (firstToken->type) {
    case TokenType::RUN:
      program.run();
      break;
    case TokenType::LIST:
      program.list();
      break;
    case TokenType::CLEAR:
      program.clear();
      break;
    case TokenType::QUIT:
      shouldQuit = true;
      break;
    case TokenType::HELP:
      break;
    default:
      return false; // 不是立即执行命令
  }

  return true;
}

int main() {
  Lexer lexer;
  Parser parser;
  Program program;
  bool shouldQuit = false;
  std::string line;
  while (!shouldQuit) {
    if (!std::getline(std::cin, line)) {
      break; // EOF或读取错误
    }

    if (line.empty()) {
      continue;
    }

    try {
      // 检查是否为立即执行命令
      if (isImmediateCommand(line)) {
        if (handleImmediateCommand(line, program, shouldQuit)) {
          continue;
        }
      }

      // 词法分析
      TokenStream tokens = lexer.tokenize(line);
      // 语法分析
      ParsedLine parsedLine = parser.parseLine(tokens, line);
      // 处理解析结果
      bool hasLine = parsedLine.hasLine();
      int lineNumber = parsedLine.getLine();
      Statement* stmt = parsedLine.fetchStatement();
      if (hasLine) {
        //有行号的情况
        if (stmt) {
          // 添加或替换语句
          program.addStmt(lineNumber, stmt);
        } else {
          // 删除语句（只有行号没有语句）
          program.removeStmt(lineNumber);
        }
      } else {
        // 没有行号的情况 - 立即执行语句
        if (stmt) {
          try {
            program.execute(stmt);
          } catch (const BasicError& e) {
            std::cout << e.message() << std::endl;
          }
          // 立即执行语句需要手动删除
          delete stmt;
        } else {
          throw BasicError("Empty statement");
        }
      }

    } catch (const BasicError& e) {
      std::cout << e.message() << std::endl;
    } catch (const std::exception& e) {
      std::cout << "Unexpected error: " << e.what() << std::endl;
    }
  }
  return 0;
}
// Created by Lenovo on 2025/11/19.
//
