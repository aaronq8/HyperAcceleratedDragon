#pragma once

#include <ast.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

namespace x64_AST {
class x64_ast_node {
public:
  virtual ~x64_ast_node() = default;
  virtual void print(std::ostream &os, int indent = 0) const = 0;

protected:
  static void do_indent(std::ostream &os, int n) {
    for (int i = 0; i < n; ++i)
      os << "  ";
  }
};

class x64_identifier_node : public x64_ast_node {
public:
  explicit x64_identifier_node(const std::string &name)
      : name_{std::move(name)} {}

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_identifier_node: " << name_ << "\n";
  }

private:
  const std::string name_;
};

class x64_inst_node : public x64_ast_node {
public:
  x64_inst_node() = default;
};

class x64_func_node : public x64_ast_node {
public:
  x64_func_node(std::unique_ptr<x64_identifier_node> func_name,
                std::vector<std::unique_ptr<x64_inst_node>> insts)
      : func_name_{std::move(func_name)}, insts_{std::move(insts)} {}

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_func_node\n";

    if (func_name_) {
      do_indent(os, indent + 1);
      os << "func_name:\n";
      func_name_->print(os, indent + 2);
    }

    do_indent(os, indent + 1);
    os << "instructions:\n";
    for (const auto &inst : insts_) {
      if (inst) {
        inst->print(os, indent + 2);
      }
    }
  }

private:
  std::unique_ptr<x64_identifier_node> func_name_;
  std::vector<std::unique_ptr<x64_inst_node>> insts_;
};

class x64_operand_node : public x64_ast_node {
public:
  x64_operand_node() = default;
};

class x64_mov_inst_node : public x64_inst_node {
public:
  x64_mov_inst_node(std::unique_ptr<x64_operand_node> src,
                    std::unique_ptr<x64_operand_node> dest)
      : src_{std::move(src)}, dest_{std::move(dest)} {}

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_mov_inst_node\n";

    if (src_) {
      do_indent(os, indent + 1);
      os << "src:\n";
      src_->print(os, indent + 2);
    }

    if (dest_) {
      do_indent(os, indent + 1);
      os << "dest:\n";
      dest_->print(os, indent + 2);
    }
  }

private:
  std::unique_ptr<x64_operand_node> src_, dest_;
};

class x64_ret_inst_node : public x64_inst_node {
public:
  x64_ret_inst_node() = default;

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_ret_inst_node\n";
  }
};

class x64_imm_node : public x64_operand_node {
public:
  x64_imm_node(int32_t val) : val_{val} {}

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_imm_node: " << val_ << "\n";
  }

private:
  int32_t val_;
};

class x64_reg_node : public x64_operand_node {
public:
  explicit x64_reg_node(std::string reg_name) : reg_name_{reg_name} {}

  void print(std::ostream &os, int indent = 0) const override {
    do_indent(os, indent);
    os << "x64_reg_node: " << reg_name_ << "\n";
  }

private:
  const std::string reg_name_;
};

class x64_program_node : public x64_ast_node {
public:
  x64_program_node(std::unique_ptr<x64_func_node> func)
      : func_{std::move(func)} {}

  void print(std::ostream &os, int indent = 0) const override {
    os << "--------------x64_AST------------\n";
    do_indent(os, indent);
    os << "x64_program_node\n";

    if (func_) {
      func_->print(os, indent + 1);
    }
    os << "----------------------------------\n";
  }

private:
  std::unique_ptr<x64_func_node> func_;
};
/*
program = Program(function_definition)
function_definition = Function(identifier name, instruction* instructions)
instruction = Mov(operand src, operand dst) | Ret
operand = Imm(int) | Register
*/
class x64_translater {
public:
  x64_translater() = default;

  std::unique_ptr<x64_program_node>
      parse_x64_prog_node(std::unique_ptr<AST::program_node>);

  std::unique_ptr<x64_func_node>
      parse_x64_func_node(std::unique_ptr<AST::func_node>);

  std::unique_ptr<x64_identifier_node>
      parse_x64_identifier_node(std::unique_ptr<AST::identifier_token_node>);

  std::unique_ptr<x64_reg_node> parse_x64_reg_node(std::string reg_name);

  std::unique_ptr<x64_imm_node>
      parse_x64_imm_node(std::unique_ptr<AST::const_token_node>);

  std::unique_ptr<x64_ret_inst_node>
      parse_x64_ret_inst_node(std::unique_ptr<AST::ret_node>);

  std::unique_ptr<x64_mov_inst_node>
      parse_x64_mov_inst_node(std::unique_ptr<AST::exp_node>);
};

} // namespace x64_AST