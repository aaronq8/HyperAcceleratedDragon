#pragma once
#include <string>

class CompilerDriver {
public:
  CompilerDriver(const std::string &input_file_path);
  // disable copying and std::move
  CompilerDriver(const CompilerDriver &) = delete;
  CompilerDriver &operator=(const CompilerDriver &) = delete;
  CompilerDriver(CompilerDriver &&) = delete;
  CompilerDriver &operator=(CompilerDriver &&) = delete;
  // main driver method
  int32_t run();

private:
  int32_t preprocess();
  // just stub response for now..
  int32_t compile();
  int32_t link();
  std::string get_file_name();
  void cleanup(int32_t pp_code, int32_t cp_code, int32_t l_code);
  const std::string input_file_path_;
  const std::string file_name_;
};