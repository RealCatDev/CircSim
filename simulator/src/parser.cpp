#include "parser.hpp"

namespace CircSim {

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
    advance();
    std::cout << m_Tok.type << " : " << m_Tok.value << std::endl;
    while (m_Tok.type == TOKN_COMMA) {
      std::cout << peek(1).type << std::endl;
      eat(TOKN_COMMA, "bro");
      eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
      res->value += m_Tok.value;
      advance();
    }
    std::cout << m_Tok.type << " : " << m_Tok.value << std::endl;

    return res;
  }

  std::unique_ptr<AstNode> Parser::ParseOut() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();
    res->type = AST_OUT;

    eat(TOKN_OUT, "Expected OUT but instead got \"" + m_Tok.value + "\"!");
    eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
    res->value += m_Tok.value;
    while (peek(1).type == TOKN_COMMA) {
      eat(TOKN_COMMA);
      eat(TOKN_ID, "Expected ID but instead got \"" + m_Tok.value + "\"!");
      res->value += ", " + m_Tok.value;
    }
    
    return res;
  }

  std::unique_ptr<AstNode> Parser::ParseBody() {
    std::unique_ptr<AstNode> res = std::make_unique<AstNode>();

    Token tok = advance();
    
    if (tok.type != TOKN_TRUTH && tok.type != TOKN_LOGIC) throw std::runtime_error("Expected circuit body (TRUTH or LOGIC) but instead got \"" + tok.value + "\"");

    res->type = (AstType)tok.type;
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
        if (onOutput) throw std::runtime_error("Unexpected token \"|\"!");
        onOutput = true;
      } else {
        if (onOutput) outs++;
        else ins++;
        eat(TOKN_ID, "bruh");
        std::unique_ptr<AstNode> column = std::make_unique<AstNode>();
        column->type = AST_TRUTH_COL;
        column->value = (int)onOutput;
        res->children.push_back(std::move(column));
      }
    }
    eat(TOKN_SEMI, "bruh, how have you managed to trick my program that bad?");
    
    while (m_Tok.type == TOKN_ID) {

    }

    return res;
  }
  
  std::unique_ptr<AstNode> Parser::ParseLogic() {
    return nullptr;
  }

}