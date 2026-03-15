#include "compiler_driver.hpp"
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <had_compiler.hpp>
#include <iostream>
#include <maximal_munch_lexer.hpp>
#include <vector>

CompilerDriver::CompilerDriver(const std::string &input_file_path)
    : input_file_path_{input_file_path}, file_name_{get_file_name()} {}

std::string CompilerDriver::read_file_to_string(const std::string &path) {
  std::ifstream file(path);

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

void CompilerDriver::cleanup(int32_t pp_code, int32_t cp_code, int32_t l_code) {
  namespace fs = std::filesystem;

  auto remove_file = [](const std::string &f) {
    std::error_code ec;
    fs::remove(f, ec);
  };

  if (!pp_code)
    remove_file(file_name_ + ".i");
  if (!cp_code)
    remove_file(file_name_ + ".s");
  if (!l_code)
    remove_file(file_name_);
}

int32_t CompilerDriver::run() {
  // maybe we need to change this to keep track of return code per stage
  // adding new stages will be annoying with this code so need to refactor
  const std::vector<std::function<int32_t()>> steps = {
      [this] { return preprocess(); }, [this] { return compile(); },
      [this] { return link(); }};
  std::vector<int32_t> return_codes{1, 1, 1};
  for (int i = 0; i < steps.size(); i++) {
    return_codes[i] = steps[i]();
    if (return_codes[i]) {
      // clean up current and all previous stages
      cleanup(return_codes[0], return_codes[1], return_codes[2]);
      return 1;
    }
  }
  // clean up post success
  cleanup(return_codes[0], return_codes[1], return_codes[2]);
  return 0;
}

std::string CompilerDriver::get_file_name() {
  std::filesystem::path path{input_file_path_};
  return path.stem().string();
}

int32_t CompilerDriver::preprocess() {
  if (file_name_.empty())
    return 1;
  std::cout << "file name : " << file_name_ << "\n";
  std::string cmd =
      "gcc -E -P " + input_file_path_ + " -o " + file_name_ + ".i";
  return std::system(cmd.c_str());
}

int32_t CompilerDriver::compile() {
  const std::string pre_process_file = file_name_ + ".i";
  std::string input_src = read_file_to_string(pre_process_file);
  HADCompiler compiler{};
  if (!compiler.lex(input_src)) {
    return 1;
  }

  const std::string asm_code =
      R"(	.file	"return_2.c"
	.text
	.globl	main
	.type	main, @function
main:
	movl	$2, %eax
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
)";
  const std::string asm_file_name = file_name_ + ".s";
  std::ofstream out(asm_file_name);
  if (!out) {
    std::cerr << "Failed to open file: " << asm_file_name << "\n";
    return 1;
  }
  out << asm_code;
  out.close();

  std::cout << "Wrote assembly to " << asm_file_name << "\n";
  return 0;
}

int32_t CompilerDriver::link() {
  const std::string cmd = "gcc " + file_name_ + ".s" + " -o " + file_name_;
  return std::system(cmd.c_str());
}