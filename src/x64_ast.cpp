#include "x64_ast.hpp"
#include <iostream>
#include <memory>

std::unique_ptr<x64_AST::x64_program_node>
x64_AST::x64_translater::parse_x64_prog_node(
    std::unique_ptr<AST::program_node> cur_ptr) {
  auto x64_func_node = parse_x64_func_node(std::move(cur_ptr->func_def_));
  return std::make_unique<x64_AST::x64_program_node>(std::move(x64_func_node));
}

std::unique_ptr<x64_AST::x64_func_node>
x64_AST::x64_translater::parse_x64_func_node(
    std::unique_ptr<AST::func_node> cur_ptr) {
  auto x64_identifier_node =
      parse_x64_identifier_node(std::move(cur_ptr->identifier_));
  // hardcoding mov and ret
  std::vector<std::unique_ptr<x64_inst_node>> insts{};
  insts.push_back(
      parse_x64_mov_inst_node(std::move(cur_ptr->stmt_->ret_->exp_)));
  insts.push_back(parse_x64_ret_inst_node(std::move(cur_ptr->stmt_->ret_)));
  return std::make_unique<x64_AST::x64_func_node>(
      std::move(x64_identifier_node), std::move(insts));
}

std::unique_ptr<x64_AST::x64_mov_inst_node>
x64_AST::x64_translater::parse_x64_mov_inst_node(
    std::unique_ptr<AST::exp_node> cur_ptr) {
  // since stmt always return x....we will hardcode mov, EAX and ret instruction
  auto src = parse_x64_imm_node(std::move(cur_ptr->const_node_));
  auto dest = parse_x64_reg_node("EAX");
  return std::make_unique<x64_AST::x64_mov_inst_node>(std::move(src),
                                                      std::move(dest));
}

std::unique_ptr<x64_AST::x64_reg_node>
x64_AST::x64_translater::parse_x64_reg_node(std::string reg_name) {
  return std::make_unique<x64_AST::x64_reg_node>(reg_name);
}

std::unique_ptr<x64_AST::x64_imm_node>
x64_AST::x64_translater::parse_x64_imm_node(
    std::unique_ptr<AST::const_token_node> cur_ptr) {
  return std::make_unique<x64_AST::x64_imm_node>(cur_ptr->val_);
}

std::unique_ptr<x64_AST::x64_identifier_node>
x64_AST::x64_translater::parse_x64_identifier_node(
    std::unique_ptr<AST::identifier_token_node> cur_ptr) {
  return std::make_unique<x64_AST::x64_identifier_node>(cur_ptr->val_);
}

std::unique_ptr<x64_AST::x64_ret_inst_node>
x64_AST::x64_translater::parse_x64_ret_inst_node(
    std::unique_ptr<AST::ret_node> cur_ptr) {
  return std::make_unique<x64_AST::x64_ret_inst_node>();
}
