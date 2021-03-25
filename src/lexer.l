%{
  #include "../src/lexer.h"
  #include "parser.hpp"

  typedef yy::parser::token token;

  #define yyterminate() yy::parser::make_END(loc);

  static yy::location loc;
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="lexer"

%%
%%
