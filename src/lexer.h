#pragma once

#if !defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer RV_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type lexer::get_next_token()

#include "parser.hpp"

class lexer : public yyFlexLexer {
public:
  lexer(std::istream* in)
      : yyFlexLexer(in)
  {
  }

  virtual yy::parser::symbol_type get_next_token();
  virtual ~lexer() { }

private:
};
