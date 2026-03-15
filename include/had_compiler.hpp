#pragma once

#include "token.hpp"
#include <string>
#include <vector>
class HADCompiler {
public:
  bool lex(const std::string &);
  HADCompiler() = default;

private:
  void pp_tokens();
  std::vector<token> tokens_;
};