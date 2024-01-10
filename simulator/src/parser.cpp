#include "parser.hpp"

namespace CircSim {

  Parser::Parser(std::unique_ptr<Lexer> lexer) : m_Tokens(lexer->GetTokens()) {}

  std::unique_ptr<AstNode> Parser::Parse() {
    std::unique_ptr<AstNode> root = std::make_unique<AstNode>();
    root->type = AST_ROOT;
    while (m_Tok.type != TOKN_EOF) root->children.push_back(ParseCircuit());
    return root;
  }

  std::unique_ptr<AstNode> Parser::ParseCircuit() {
    std::unique_ptr<AstNode> circuit = std::make_unique<AstNode>();
    circuit->type = AST_CIRCUIT;

    eat(TOKN_CIRCUIT, "Expected CIRCUIT but instead got \"" + m_Tok.value + "\"!");
    eat(TOKN_LPAREN, "Expected ( but instead got \"" + m_Tok.value + "\"!");
    eat(TOKN_ID, "Expected circuit name (TOKN_ID) but instead got \"" + m_Tok.value + "\"!");
    circuit->value = m_Tok.value;
    eat(TOKN_RPAREN, "Expected ) but instead got \"" + m_Tok.value + "\"!");

    circuit->children.push_back(ParseIn());
    circuit->children.push_back(ParseOut());
    circuit->children.push_back(ParseBody());

    return circuit;
  }

  std::unique_ptr<AstNode> Parser::ParseIn() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();
    res->type = AST_IN;
    res->value = "";

    eat(TOKN_IN, "Expected IN but instead got \"" + m_Tok.value + "\"!");
    eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
    res->value += m_Tok.value;

    while (peek(0).type == TOKN_COMMA) {
      advance();
      eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
      res->value += " " + m_Tok.value;
    }

    return res;
  }

  std::unique_ptr<AstNode> Parser::ParseOut() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();
    res->type = AST_OUT;

    eat(TOKN_OUT, "Expected OUT but instead got \"" + m_Tok.value + "\"!");
    eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
    res->value += m_Tok.value;

    while (peek(0).type == TOKN_COMMA) {
      advance();
      eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
      res->value += " " + m_Tok.value;
    }

    return res;
  }

  std::unique_ptr<AstNode> Parser::ParseBody() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();

    Token tok = advance();
    
    if (tok.type != TOKN_TRUTH && tok.type != TOKN_LOGIC) throw std::runtime_error("Expected circuit body (TRUTH or LOGIC) but instead got \"" + tok.value + "\"");

    res->type = static_cast<AstType>(tok.type);
    eat(tok.type);
    eat(TOKN_COLON);
    if (tok.type == TOKN_TRUTH) {
      res->children.push_back(ParseTruth());
    } else {
      res->children.push_back(ParseLogic());
    }

    return res;
  }

  std::unique_ptr<AstNode> Parser::ParseTruth() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();
    res->type = AST_TRUTH_VALUE;
    bool onOutput = false;
    int ins = 0, outs = 0;

    while (m_Tok.type != TOKN_SEMI) {
      if (m_Tok.type == TOKN_PIPE) {
        advance();
        if (onOutput) throw std::runtime_error("Unexpected token \"|\"!");
        onOutput = true;
      } else {
        if (onOutput) outs++;
        else ins++;
        if (m_Tok.type != TOKN_ID) throw std::runtime_error("Bad token!");
        advance();
        std::unique_ptr<AstNode> column = std::make_unique<AstNode>();
        column->type = AST_TRUTH_COL;
        column->value = onOutput?"i":"o";
        res->children.push_back(std::move(column));
      }
    }
    advance();
    
    bool loop = true, first = true;
    while (loop) {
      if (!first) {
        if (m_Tok.type != TOKN_COMMA) { loop = false; }
        else advance();
        std::cout << m_Tok.type << std::endl;
      }

      std::vector<std::string> inputs{}, outputs{};
      for (uint32_t i = 0; i < ins; ++i) {
        if (m_Tok.type != TOKN_NUMBER) throw std::runtime_error("DON'T!");
        inputs.push_back(m_Tok.value);
        advance();
      }

      if (m_Tok.type != TOKN_PIPE) throw std::runtime_error("no!");
      advance();

      for (uint32_t i = 0; i < outs; ++i) {
        std::cout << m_Tok.type << std::endl;
        if (m_Tok.type != TOKN_NUMBER) throw std::runtime_error("DON'T!");
        outputs.push_back(m_Tok.value);
        advance();
      }
      first = false;
    }

    return res;
  }
  
  std::unique_ptr<AstNode> Parser::ParseLogic() {
    return nullptr;
  }

  Token Parser::advance() {
    return m_Tok = m_Tokens[m_Index++];
  }

  Token Parser::peek(int off) {
    return m_Tokens[m_Index + off];
  }

  bool Parser::eat(TokenType type) {
    return advance().type == type;
  }

  void Parser::eat(TokenType type, const std::string& errMsg) {
    if (!eat(type)) throw std::runtime_error(errMsg);
  }

}