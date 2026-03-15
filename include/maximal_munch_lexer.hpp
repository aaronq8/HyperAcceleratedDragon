#pragma once

#include "token.hpp"
#include <vector>
class mm_lexer {
public:
  explicit mm_lexer(const std::string &input_source);
  bool tokenize();
  const std::vector<token> &get_tokens();

private:
  bool is_space(const char &);
  TOKEN_TYPE get_single_char_token(const char &);
  TOKEN_TYPE identifier_to_keyword(std::string_view s);
  std::vector<token> tokens_;
  const std::string input_source_;
  const int token_size_avg_;
};