#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <compiler_driver.hpp>
#include <iostream>

int main(int argc, char **argv) {
  namespace po = boost::program_options;
  po::options_description desc{"Allowed options"};
  std::string file_path{};
  const std::string input_option{"input"};
  desc.add_options()(input_option.c_str(),
                     po::value<std::string>(&file_path)->required(),
                     "Input Source File");

  po::positional_options_description pos_opt{};
  pos_opt.add(input_option.c_str(), 1);
  po::variables_map vmap{};
  // parse argv using po
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(desc)
                .positional(pos_opt)
                .run(),
            vm);
  po::notify(vm);

  std::cout << "file_path : " << file_path << "\n";
  CompilerDriver compiler_driver{file_path};
  int32_t code = compiler_driver.run();
  std::cout << "compiler driver returned : " << code << "\n";
  return code;
}
