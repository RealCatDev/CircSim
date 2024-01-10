#include "lexer.hpp"

#include <fstream>

namespace CircSim {

  Lexer::Lexer(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) throw std::runtime_error("Failed to open file!");
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    m_Content = std::string(size, ' ');
    file.seekg(0);
    file.read(&m_Content[0], size);
    m_Char = m_Content[m_Index];
  }

  std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens{};

    while (m_Char != '\0' && m_Index < m_Content.size()) {
      Token tok{};
      skipSpaces();
      tok.line = m_Line;
      tok.col = m_Col;
      if (isdigit(m_Char)) {
        std::string val{};
        while (isdigit(m_Char)) {
          val += advance();
        }
        tok.type = TOKN_NUMBER;
        tok.value = val;
      }
      else if (isalnum(m_Char)) {
        std::string val{};
        while (isalnum(m_Char)) {
          val += advance();
        }
        tok.value = val;
        if (val == "IN")
          tok.type = TOKN_IN;
        else if (val == "OUT")
          tok.type = TOKN_OUT;
        else if (val == "CIRCUIT")
          tok.type = TOKN_CIRCUIT;
        else if (val == "TRUTH")
          tok.type = TOKN_TRUTH;
        else if (val == "LOGIC")
          tok.type = TOKN_LOGIC;
        else
          tok.type = TOKN_ID;
      }
      else {
        switch (m_Char)
        {
        case '(': {
          tok = getTok(TOKN_LPAREN);
        } break;
        case ')': {
          tok = getTok(TOKN_RPAREN);
        } break;
        case ',': {
          tok = getTok(TOKN_COMMA);
        } break;
        case ':': {
          tok = getTok(TOKN_COLON);
        } break;
        case '|': {
          tok = getTok(TOKN_PIPE);
        } break;
        case '=': {
          tok = getTok(TOKN_EQUALS);
        } break;
        
        default: throw std::runtime_error("Unexpected character: '" + std::string(1, m_Char) + "'!");
        }
      }
      tokens.push_back(tok);
    }

    {
      Token eof{};
      eof.type = TOKN_EOF;
      eof.value = m_Char;
      tokens.push_back(eof);
    }

    return tokens;
  }

}