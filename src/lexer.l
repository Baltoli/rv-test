%{
  #include "parser.hpp"

  #include <iostream>
  using namespace std;
  extern int yylex();
%}

%option noinput nounput noyywrap nodefault

%%

print  { return PRINT; }
[0-9]+ { yylval.ival = atoi(yytext); return INT; }

"(" { return LPAREN; }
")" { return RPAREN; }
";" { return SEMICOLON; }

"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }

\n    { return ENDL; }

. { std::cout << "Bad character: " << yytext << '\n'; }

%%
