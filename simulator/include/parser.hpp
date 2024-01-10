#ifndef CIRCSIM_PARSER_HPP_
#define CIRCSIM_PARSER_HPP_

#include "lexer.hpp"

#include <unordered_map>

namespace CircSim {

  enum AstType {
    AST_CIRCUIT = 0,
    AST_IN = 1, AST_OUT = 2,
    AST_TRUTH = 3, AST_LOGIC = 4,
    AST_CIRCUIT_CALL = 5, AST_VAR_ASSIGNMENT = 6
  };

  struct AstNode {
    AstType type;
    std::string value;
    std::vector<std::shared_ptr<AstNode>> children;
  };

  class Parser {
  public:
    Parser(std::unique_ptr<Lexer> lexer) : m_Tokens(lexer->Tokenize()) {}

    std::unique_ptr<AstNode> Parse() {
      std::unique_ptr<AstNode> root;
      


      return root;
    }

  private:
    std::vector<Token> m_Tokens{};
    int m_Index = 0;
  };

}

#endif // CIRCSIM_PARSER_HPP_