%{
  #include "../src/lexer.h"
  #include "parser.hpp"

  #include <iostream>
  #include <cstdlib>

  typedef yy::parser::token token;

  #define yyterminate() yy::parser::make_END(loc);

  static yy::location loc;
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="lexer"
%option prefix="RV_"

%%
  
print { return yy::parser::make_PRINT(yy::location()); }

"(" { return yy::parser::make_LPAREN(yy::location()); }
")" { return yy::parser::make_RPAREN(yy::location()); }
";" { return yy::parser::make_SEMICOLON(yy::location()); }

"+" { return yy::parser::make_ADD(yy::location()); }
"-" { return yy::parser::make_SUB(yy::location()); }
"*" { return yy::parser::make_MUL(yy::location()); }
"/" { return yy::parser::make_DIV(yy::location()); }

\n { return yy::parser::make_ENDL(yy::location()); }

[0-9]+ {
  int64_t num = std::strtol(yytext, nullptr, 10);
  return yy::parser::make_INT(num, yy::location());
}

  /* 
   * Ignore any characters we don't know about - this isn't a robust strategy in
   * general but is fine for this limited use case.
   */
.           { }
            
<<EOF>>     { return yyterminate(); }

%%
