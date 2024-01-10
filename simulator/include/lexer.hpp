#ifndef CIRCSIM_LEXER_HPP_
#define CIRCSIM_LEXER_HPP_

#include <iostream>
#include <vector>
#include <cctype> // Include for isspace

namespace CircSim {

  enum TokenType {
    TOKN_ID = 0, TOKN_NUMBER = 1,
    TOKN_COLON = 2, TOKN_COMMA = 3, TOKN_EQUALS = 4, TOKN_PIPE = 5,
    TOKN_LPAREN = 6, TOKN_RPAREN = 7,

    TOKN_CIRCUIT = 8, TOKN_IN = 9, TOKN_OUT = 10, TOKN_TRUTH = 11, TOKN_LOGIC = 12,
    TOKN_EOF = -1
  };

  struct Token {
    TokenType type;
    std::string value;
    int line, col;

    Token() = default;
    Token(const Token& other) : type(other.type), value(other.value), line(other.line), col(other.col) {}
  };

  class Lexer {
  public:
    Lexer() = default;
    Lexer(const std::string &filepath);

    void SetContent(const std::string &content);
    std::vector<Token> GetTokens();
  private:
    Token GetTok();
    void advance();
    void skipSpaces();
  private:
    std::string m_Content = "";
    char m_Char = ' ';
    size_t m_Index = 0, m_Line = 1, m_Column = 0;
  };

}

#endif // CIRCSIM_LEXER_HPP_
