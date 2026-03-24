#pragma once

#include "token.hpp"
#include <deque>
#include <memory>
#include <string>
#include <vector>
/*
ASDL for this AST....for now
program = Program(function_definition)
function_definition = Function(identifier name, statement body)
statement = Return(exp)
exp = Constant(int) | Unary(unary_operator, exp)
unary_operator = Complement | Negate
*/
namespace AST {

class ast_node {
public:
  virtual ~ast_node() = default;
  virtual void print(std::ostream &os, int indent = 0) const = 0;

protected:
  static void indent(std::ostream &os, int n) {
    for (int i = 0; i < n; ++i)
      os << "  ";
  }
};

class ast_token_node : public ast_node {
public:
  ast_token_node(TOKEN_TYPE token_type) : token_type_{token_type} {};

  const TOKEN_TYPE token_type_;
};

class exp_node : public ast_node {
public:
  virtual ~exp_node() = default;
};

// *_token_node constructs map to tokens...therefore terminal symbols
class const_token_node : public ast_token_node, public exp_node {
public:
  const_token_node(const int32_t val)
      : val_{val}, ast_token_node{TOKEN_TYPE::CONSTANT} {}

  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "Constant(" << val_ << ")\n";
  }

  const int32_t val_;
};

class identifier_token_node : public ast_token_node {
public:
  identifier_token_node(const std::string &val)
      : val_{val}, ast_token_node{TOKEN_TYPE::IDENTIFIER} {}

  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "Identifier(" << val_ << ")\n";
  }

  const std::string val_;
};

class unary_op_node : public ast_node {
public:
  virtual ~unary_op_node() = default;
};

class stmt_node : public ast_node {
public:
  virtual ~stmt_node() = default;
};

class complement_token_node : public ast_token_node, public unary_op_node {
public:
  complement_token_node() : ast_token_node{TOKEN_TYPE::UNOP_COMPLEMENT} {}
  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "ComplementTokenNode\n";
  }
};

class negate_token_node : public ast_token_node, public unary_op_node {
public:
  negate_token_node() : ast_token_node{TOKEN_TYPE::UNOP_NEGATE} {}
  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "NegateTokenNode\n";
  }
};

class unary_node : public exp_node {
public:
  unary_node(std::unique_ptr<unary_op_node> op_node,
             std::unique_ptr<exp_node> exp_node)
      : op_node_{std::move(op_node)}, exp_node_{std::move(exp_node)} {}

  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "Unary\n";
    op_node_->print(os, indent_lvl + 1);
    exp_node_->print(os, indent_lvl + 1);
  }

private:
  std::unique_ptr<unary_op_node> op_node_;
  std::unique_ptr<exp_node> exp_node_;
};

class ret_node : public stmt_node {
public:
  explicit ret_node(std::unique_ptr<exp_node> exp) : exp_{std::move(exp)} {}

  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "Return\n";
    exp_->print(os, indent_lvl + 1);
  }

  std::unique_ptr<exp_node> exp_;
};

class func_node : public ast_node {
public:
  explicit func_node(std::unique_ptr<identifier_token_node> identifier,
                     std::unique_ptr<stmt_node> stmt)
      : identifier_{std::move(identifier)}, stmt_{std::move(stmt)} {}

  void print(std::ostream &os, int indent_lvl) const override {
    indent(os, indent_lvl);
    os << "Function\n";

    identifier_->print(os, indent_lvl + 1);
    stmt_->print(os, indent_lvl + 1);
  }

  std::unique_ptr<identifier_token_node> identifier_;
  std::unique_ptr<stmt_node> stmt_;
};

class program_node : public ast_node {
public:
  explicit program_node(std::unique_ptr<func_node> func_def)
      : func_def_{std::move(func_def)} {}

  void print(std::ostream &os, int indent_lvl = 0) const override {
    os << "----------AST----------\n";
    indent(os, indent_lvl);
    os << "Program\n";

    func_def_->print(os, indent_lvl + 1);
    os << "---------------------------\n";
  }

  std::unique_ptr<func_node> func_def_;
};
/*
<program> ::= <function>
<function> ::= "int" <identifier> "(" "void" ")" "{" <statement> "}"
<statement> ::= "return" <exp> ";"
<exp> ::= <int> | <unop> <exp> | "(" <exp> ")"
<unop> ::= "-" | "~"
<identifier> ::= ? An identifier token ?
<int> ::= ? A constant token ?
*/
class ast_parser {
  // TODO : implement proper syntactic error messages with line numbers
public:
  explicit ast_parser(const std::vector<token> tokens);
  template <typename NodeT> std::unique_ptr<NodeT> parse_node();

private:
  bool expect_token(const TOKEN_TYPE &token_type);
  bool consume(TOKEN_TYPE token_type, std::string_view error_msg);

  template <typename NodeT>
  std::unique_ptr<NodeT> parse_required(std::string_view error_msg);
  std::deque<token> tokens_;
};

} // namespace AST