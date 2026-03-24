#pragma once

#include <ostream>
#include <string>

enum TOKEN_TYPE {
  IDENTIFIER,
  CONSTANT,
  KEYWORD_INT,
  KEYWORD_VOID,
  KEYWORD_RETURN,
  PARANTHESIS_OPEN,
  PARANTHESIS_CLOSED,
  BRACE_OPEN,
  BRACE_CLOSE,
  SEMICOLON,
  ERROR,
  UNOP_COMPLEMENT,
  UNOP_NEGATE,
  UNOP_DECREMENT
};

struct token {
  std::string lexeme_;
  TOKEN_TYPE token_type_;
  token(const std::string lexeme, const TOKEN_TYPE token_type)
      : lexeme_{lexeme}, token_type_{token_type} {}
};

inline std::ostream &operator<<(std::ostream &os, TOKEN_TYPE type) {
  switch (type) {
  case IDENTIFIER:
    os << "IDENTIFIER";
    break;
  case CONSTANT:
    os << "CONSTANT";
    break;
  case KEYWORD_INT:
    os << "KEYWORD_INT";
    break;
  case KEYWORD_VOID:
    os << "KEYWORD_VOID";
    break;
  case KEYWORD_RETURN:
    os << "KEYWORD_RETURN";
    break;
  case PARANTHESIS_OPEN:
    os << "PARANTHESIS_OPEN";
    break;
  case PARANTHESIS_CLOSED:
    os << "PARANTHESIS_CLOSED";
    break;
  case BRACE_OPEN:
    os << "BRACE_OPEN";
    break;
  case BRACE_CLOSE:
    os << "BRACE_CLOSE";
    break;
  case SEMICOLON:
    os << "SEMICOLON";
    break;
  case ERROR:
    os << "ERROR";
    break;
  case UNOP_COMPLEMENT:
    os << "UNOP_COMPLEMENT";
    break;
  case UNOP_NEGATE:
    os << "UNOP_NEGATE";
    break;
  case UNOP_DECREMENT:
    os << "UNOP_DECREMENT";
    break;
  default:
    os << "UNKNOWN_TOKEN";
    break;
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const token &t) {
  os << "Token{type=" << t.token_type_ << ", lexeme=\"" << t.lexeme_ << "\"}";
  return os;
}