#ifndef CIRCSIM_LEXER_HPP_
#define CIRCSIM_LEXER_HPP_

#include <iostream>
#include <vector>

namespace CircSim {

  enum TokenType {
    TOKN_ID = 0, TOKN_NUMBER = 1,
    TOKN_COLON = 2, TOKN_COMMA = 3, TOKN_EQUALS = 4, TOKN_PIPE = 5,
    TOKN_LPAREN = 6, TOKN_RPAREN = 7,  // Added parentheses

    TOKN_CIRCUIT = 8, TOKN_IN = 9, TOKN_OUT = 10, TOKN_TRUTH = 11, TOKN_LOGIC = 12,
    TOKN_EOF = -1
  };

  struct Token {
    TokenType type;
    
    std::string value;
    int line, col;
  };

  class Lexer {
  public:
    Lexer(const std::string &filepath);
    ~Lexer() = default;
    std::vector<Token> Tokenize();
  private:
    std::string m_Content{};
    char m_Char = ' ';
    int m_Index = 0, m_Line = 1, m_Col = 0;

    void skipSpaces() {
      while (m_Char == ' ' || m_Char == '\t' || m_Char == '\n' || iswspace(m_Char) || isspace(m_Char) || m_Char == '\r') advance();
    }

    char advance() {
      m_Col++;
      m_Char = m_Content[++m_Index];
      if (m_Char == '\n') { m_Line++; m_Col = 0; }
      return m_Char;
    }

    char peek(int off = 0) {
      if (m_Index + off >= m_Content.size()) return '\0';
      return m_Content[m_Index + off];
    }

    Token getTok(TokenType type) {
      std::string val = std::string(1, advance());
      Token tok{};
      tok.type = type;
      tok.value = val;
      return tok;
    }
  };

}

#endif // CIRCSIM_LEXER_HPP_