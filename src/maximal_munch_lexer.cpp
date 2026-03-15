#include "maximal_munch_lexer.hpp"
#include <cctype>
#include <string>
#include <unordered_map>

mm_lexer::mm_lexer(const std::string &input_source)
    : input_source_{input_source}, token_size_avg_{5} {
  tokens_.reserve(input_source_.size() / token_size_avg_);
}
bool mm_lexer::is_space(const char &c) {
  return std::isspace(static_cast<unsigned char>(c));
}

const std::vector<token> &mm_lexer::get_tokens() { return tokens_; }
TOKEN_TYPE mm_lexer::get_single_char_token(const char &c) {
  switch (c) {
  case '(':
    return TOKEN_TYPE::PARANTHESIS_OPEN;
  case ')':
    return TOKEN_TYPE::PARANTHESIS_CLOSED;
  case '{':
    return TOKEN_TYPE::BRACE_OPEN;
  case '}':
    return TOKEN_TYPE::BRACE_CLOSE;
  case ';':
    return TOKEN_TYPE::SEMICOLON;
  default:
    return TOKEN_TYPE::ERROR;
  }
}

TOKEN_TYPE mm_lexer::identifier_to_keyword(std::string_view s) {
  static const std::unordered_map<std::string_view, TOKEN_TYPE> keywords = {
      {"int", TOKEN_TYPE::KEYWORD_INT},
      {"return", TOKEN_TYPE::KEYWORD_RETURN},
      {"void", TOKEN_TYPE::KEYWORD_VOID}};
  auto it = keywords.find(s);
  if (it != keywords.end())
    return it->second;
  return TOKEN_TYPE::IDENTIFIER;
}

bool mm_lexer::tokenize() {
  int cur_char = 0;
  while (cur_char < input_source_.size()) {
    // whitespace
    if (is_space(input_source_[cur_char])) {
      cur_char++;
      continue;
    }
    // 1-char tokens
    auto single_char_token = get_single_char_token(input_source_[cur_char]);
    if (single_char_token != TOKEN_TYPE::ERROR) {
      tokens_.emplace_back(std::string(1, input_source_[cur_char]),
                           single_char_token);
      cur_char++;
      continue;
    }
    // constant
    if (std::isdigit(input_source_[cur_char])) {
      int start_char = cur_char;
      for (; cur_char < input_source_.size() &&
             std::isdigit(input_source_[cur_char]);
           cur_char++) {
      }
      tokens_.emplace_back(
          input_source_.substr(start_char, cur_char - start_char),
          TOKEN_TYPE::CONSTANT);
      continue;
    }
    // identifier and keyword
    if (std::isalpha(input_source_[cur_char]) ||
        input_source_[cur_char] == '_') {
      int start_char = cur_char;
      for (; cur_char < input_source_.size() &&
             (std::isalnum(input_source_[cur_char]) ||
              input_source_[cur_char] == '_');
           cur_char++) {
      }
      const std::string lexeme{
          input_source_.substr(start_char, cur_char - start_char)};
      // check keywords
      auto token_type = identifier_to_keyword(lexeme);
      tokens_.emplace_back(lexeme, token_type);
      continue;
    }
    // compile error
    return false;
  }
  return true;
}
