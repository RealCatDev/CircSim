#include <iostream>
#include <filesystem>

#include "lexer.hpp"
#include "parser.hpp"
#include "simulator.hpp"

using namespace CircSim;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }

  if (!std::filesystem::exists(argv[1])) {
    std::cerr << "Failed to read file \"" << argv[1] << "\"!" << std::endl;
    return 1;
  }

  std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(argv[1]);
  lexer->Tokenize();

  return 0;
}