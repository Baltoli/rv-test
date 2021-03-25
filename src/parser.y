%skeleton "lalr1.cc"
%require "3.0.2"

%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert true

%code requires {
  class lexer;
}

%locations
%initial-action
{
};

%define parse.trace
%define parse.error verbose

%lex-param { lexer &lex }
%parse-param { lexer &lex }

%code top {
  #include "../src/lexer.h"
  #include "parser.hpp"

  static yy::parser::symbol_type yylex(lexer &scanner) {
    return scanner.get_next_token();
  }
}

%token END

%%

program:

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
}
