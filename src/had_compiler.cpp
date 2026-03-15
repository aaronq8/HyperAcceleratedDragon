#include "had_compiler.hpp"
#include "maximal_munch_lexer.hpp"
#include <iostream>
bool HADCompiler::lex(const std::string &input_source) {
  mm_lexer lexer{input_source};
  bool lex_result = lexer.tokenize();
  tokens_ = lexer.get_tokens();
  //  pretty print
  if (lex_result)
    pp_tokens();
  else {
    std::cout << "invalid token detected...skipping print..\n";
  }
  return lex_result;
}

void HADCompiler::pp_tokens() {
  for (const auto &token : tokens_) {
    std::cout << token << "\n";
  }
}
