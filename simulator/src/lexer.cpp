#include "lexer.hpp"

#include <fstream>

namespace CircSim {

  Lexer::Lexer(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to read file \"" + filepath + "\"!");
      return;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    m_Content = std::string(size, ' ');
    file.seekg(0);
    file.read(&m_Content[0], size);
  }

  void Lexer::SetContent(const std::string &content) {
    m_Content = content;
  }

  std::vector<Token> Lexer::GetTokens() {
    std::vector<Token> tokens{};
    bool loop = true;
    while (loop) {
      auto tokn = GetTok();
      tokens.push_back(tokn);
      if (tokn.type == TOKN_EOF) loop = false;
    }

    return tokens;
  }

  // private:

  Token Lexer::GetTok() {
    Token tok{};
    tok.type = TOKN_EOF;
    skipSpaces();
    tok.line = m_Line;
    tok.col = m_Column;
    if (m_Index >= m_Content.size() && m_Char == '\0') return tok;

    if (isdigit(m_Char)) {
      tok.type = TOKN_NUMBER;
      tok.value = "";
      while (isdigit(m_Char)) {
        tok.value += m_Char;
        advance();
      }
    }

    else if (isalnum(m_Char)) {
      tok.type = TOKN_ID;
      tok.value = "";
      while (isalnum(m_Char) || m_Char == '_') {
        tok.value += m_Char;
        advance();
      }
    }

    else {
      switch (m_Char) {
      case '(': { tok.type = TOKN_LPAREN; } break;
      case ')': { tok.type = TOKN_RPAREN; } break;
      case ':': { tok.type = TOKN_COLON; } break;
      case ';': { tok.type = TOKN_SEMI; } break;
      case '=': { tok.type = TOKN_EQUALS; } break;
      case ',': { tok.type = TOKN_COMMA; } break;
      case '|': { tok.type = TOKN_PIPE; } break;
      default: { throw std::runtime_error(std::string("Unexpected character '") + m_Char + "' at " + std::to_string(m_Line) + ":" + std::to_string(m_Column)); }
      }
      tok.value = std::string(1, m_Char);
      advance();
    }

    return tok;
  }

  void Lexer::advance() {
    if (m_Content.size() <= m_Index && m_Char == '\0') return;
    m_Char = m_Content[m_Index++];
    m_Column++;
    if (m_Char == '\n') { m_Line++; m_Column = 0; }
  }

  void Lexer::skipSpaces() {
    while (m_Char == ' ' || m_Char == '\t' || m_Char == '\n' || iswspace(m_Char) || isspace(m_Char) || m_Char == '\r') advance();
  }

}