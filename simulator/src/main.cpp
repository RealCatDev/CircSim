#include <iostream>
#include <filesystem>

#include "lexer.hpp"
#include "parser.hpp"
#include "simulator.hpp"

#include <string>

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
  for (auto tok : lexer->GetTokens()) {
    std::cout << "Token " << tok.value << " at " << std::to_string(tok.line) << ":" << std::to_string(tok.col) << std::endl;
  }
  
  return 0;
}