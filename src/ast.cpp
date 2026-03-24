#include "ast.hpp"
#include "token.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

AST::ast_parser::ast_parser(const std::vector<token> tokens) {
  for (const auto &token : tokens) {
    tokens_.push_back(token);
  }
}

bool AST::ast_parser::expect_token(const TOKEN_TYPE &token_type) {
  return !tokens_.empty() && tokens_.front().token_type_ == token_type;
}

template <>
std::unique_ptr<AST::identifier_token_node>
AST::ast_parser::parse_node<AST::identifier_token_node>() {
  // parse identifier token
  if (tokens_.empty()) {
    return nullptr;
  }

  if (tokens_.front().token_type_ == TOKEN_TYPE::IDENTIFIER) {
    auto ident =
        std::make_unique<AST::identifier_token_node>(tokens_.front().lexeme_);
    tokens_.pop_front();
    return ident;
  }
  return nullptr;
}

// removes used tokens
bool AST::ast_parser::consume(TOKEN_TYPE token_type,
                              std::string_view error_msg) {
  if (!expect_token(token_type)) {
    std::cout << error_msg << '\n';
    return false;
  }
  tokens_.pop_front();
  return true;
}

template <>
std::unique_ptr<AST::const_token_node>
AST::ast_parser::parse_node<AST::const_token_node>() {
  // parse constant token
  if (tokens_.empty()) {
    return nullptr;
  }
  if (tokens_.front().token_type_ == TOKEN_TYPE::CONSTANT) {
    auto num = std::make_unique<AST::const_token_node>(
        std::stoi(tokens_.front().lexeme_));
    tokens_.pop_front();
    return num;
  }
  return nullptr;
}

// <unop> ::= "-" | "~"
template <>
std::unique_ptr<AST::unary_op_node>
AST::ast_parser::parse_node<AST::unary_op_node>() {
  if (tokens_.empty()) {
    return nullptr;
  }

  if (tokens_.front().token_type_ == TOKEN_TYPE::UNOP_NEGATE) {
    tokens_.pop_front();
    return std::make_unique<AST::negate_token_node>();
  }

  if (tokens_.front().token_type_ == TOKEN_TYPE::UNOP_COMPLEMENT) {
    tokens_.pop_front();
    return std::make_unique<AST::complement_token_node>();
  }

  return nullptr;
}

// <exp> ::= <int> | <unop> <exp> | "(" <exp> ")"
template <>
std::unique_ptr<AST::exp_node> AST::ast_parser::parse_node<AST::exp_node>() {
  if (tokens_.empty()) {
    return nullptr;
  }

  // <int>
  if (expect_token(TOKEN_TYPE::CONSTANT)) {
    return parse_node<AST::const_token_node>();
  }

  // <unop> <exp>
  if (expect_token(TOKEN_TYPE::UNOP_NEGATE) ||
      expect_token(TOKEN_TYPE::UNOP_COMPLEMENT)) {
    auto op =
        parse_required<AST::unary_op_node>("invalid or missing unary operator");
    if (!op) {
      return nullptr;
    }
    // recursive
    auto inner_exp = parse_required<AST::exp_node>(
        "invalid or missing expression after unary operator");
    if (!inner_exp) {
      return nullptr;
    }

    return std::make_unique<AST::unary_node>(std::move(op),
                                             std::move(inner_exp));
  }

  // "(" <exp> ")"
  if (expect_token(TOKEN_TYPE::PARANTHESIS_OPEN)) {
    tokens_.pop_front();

    auto inner_exp = parse_required<AST::exp_node>(
        "invalid or missing parenthesized expression");
    if (!inner_exp) {
      return nullptr;
    }

    if (!consume(TOKEN_TYPE::PARANTHESIS_CLOSED,
                 "missing ')' after expression")) {
      return nullptr;
    }

    return inner_exp;
  }

  return nullptr;
}

// "return" <exp> ";"
template <>
std::unique_ptr<AST::ret_node> AST::ast_parser::parse_node<AST::ret_node>() {
  if (!consume(TOKEN_TYPE::KEYWORD_RETURN, "missing return keyword")) {
    return nullptr;
  }

  auto exp = parse_required<AST::exp_node>("missing expression after return");
  if (!exp) {
    return nullptr;
  }

  if (!consume(TOKEN_TYPE::SEMICOLON, "missing ';'")) {
    return nullptr;
  }

  return std::make_unique<AST::ret_node>(std::move(exp));
}

// <statement> ::= "return" <exp> ";"
template <>
std::unique_ptr<AST::stmt_node> AST::ast_parser::parse_node<AST::stmt_node>() {
  auto ret =
      parse_required<AST::ret_node>("invalid or missing return statement");
  if (!ret) {
    return nullptr;
  }

  return ret;
}

// <function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
template <>
std::unique_ptr<AST::func_node> AST::ast_parser::parse_node<AST::func_node>() {
  if (!consume(TOKEN_TYPE::KEYWORD_INT,
               "invalid or missing function return type")) {
    return nullptr;
  }

  auto identifier = parse_required<AST::identifier_token_node>(
      "invalid or missing function name");
  if (!identifier) {
    return nullptr;
  }

  // func related syntax
  if (!consume(TOKEN_TYPE::PARANTHESIS_OPEN, "no ( in function def") ||
      !consume(TOKEN_TYPE::KEYWORD_VOID, "no void in function def") ||
      !consume(TOKEN_TYPE::PARANTHESIS_CLOSED, "no ) in function def") ||
      !consume(TOKEN_TYPE::BRACE_OPEN, "no { in function def")) {
    return nullptr;
  }

  auto stmt = parse_required<AST::stmt_node>("missing function body");
  if (!stmt) {
    return nullptr;
  }

  if (!consume(TOKEN_TYPE::BRACE_CLOSE, "missing '}' in function definition")) {
    return nullptr;
  }
  return std::make_unique<AST::func_node>(std::move(identifier),
                                          std::move(stmt));
}

// <program> ::= <function>
template <>
std::unique_ptr<AST::program_node>
AST::ast_parser::parse_node<AST::program_node>() {
  auto func =
      parse_required<AST::func_node>("invalid or missing function definition");
  if (!func) {
    return nullptr;
  }

  return std::make_unique<AST::program_node>(std::move(func));
}

template <typename NodeT>
std::unique_ptr<NodeT>
AST::ast_parser::parse_required(std::string_view error_msg) {
  auto node = parse_node<NodeT>();
  if (!node) {
    std::cout << error_msg << '\n';
  }
  return node;
}