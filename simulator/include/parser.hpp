#ifndef CIRCSIM_PARSER_HPP_
#define CIRCSIM_PARSER_HPP_

#include "lexer.hpp"

#include <memory>
#include <unordered_map>

namespace CircSim {

  enum AstType {
    AST_ROOT = 0,
    AST_IN = 1, AST_OUT = 2,
    AST_CIRCUIT = 3, AST_CIRCUIT_CALL = 4, AST_VAR_ASSIGNMENT = 5,
    AST_TRUTH = 11, AST_LOGIC = 12, // numbers for tokens (TOKN_TRUTH and TOKN_LOGIC)
    AST_TRUTH_VALUE = 13, AST_TRUTH_COL = 14, AST_TRUTH_ROW = 15
  };

  struct AstNode {
    AstType type;
    std::string value;
    std::vector<std::unique_ptr<AstNode>> children;
  };

  class Parser {
  public:
    Parser(std::unique_ptr<Lexer> lexer);

    std::unique_ptr<AstNode> Parse();
  private:
    std::vector<Token> m_Tokens{};
    Token m_Tok;
    int m_Index = 0;
  private:
    std::unique_ptr<AstNode> ParseCircuit();
    std::unique_ptr<AstNode> ParseIn();
    std::unique_ptr<AstNode> ParseOut();
    std::unique_ptr<AstNode> ParseBody();
    std::unique_ptr<AstNode> ParseTruth();
    std::unique_ptr<AstNode> ParseLogic();
  private:
    Token advance();
    Token peek(int off = 0);
    bool eat(TokenType type);
    void eat(TokenType type, const std::string& errMsg);
  };

}

#endif // CIRCSIM_PARSER_HPP_
