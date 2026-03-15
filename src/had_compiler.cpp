#include "had_compiler.hpp"
#include "RAII_profiler.hpp"
#include "ast.hpp"
#include "maximal_munch_lexer.hpp"
#include <iostream>

bool HADCompiler::lex(const std::string &input_source) {
  RAII_Profiler<std::chrono::microseconds> profiler{"LEXER"};
  mm_lexer lexer{input_source};
  bool lex_result = lexer.tokenize();
  tokens_ = lexer.get_tokens();
  //  pretty print
  if (lex_result) {
    pp_tokens();
  } else {
    std::cout << "invalid token detected...skipping print..\n";
  }
  return lex_result;
}

bool HADCompiler::generate_ast() {
  RAII_Profiler<std::chrono::microseconds> profiler{"AST"};
  AST::ast_parser ast{tokens_};
  auto ast_root = ast.parse_node<AST::program_node>();
  if (ast_root == nullptr) {
    std::cout << "syntax error detected...\n";
    return false;
  } else {
    ast_root->print(std::cout, 0);
  }
  return true;
}

void HADCompiler::pp_tokens() {
  std::cout << "--------tokens----------\n";
  for (const auto &token : tokens_) {
    std::cout << token << "\n";
  }
  std::cout << "-----------------------\n";
}
