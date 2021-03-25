%{
  #include "parser.hpp"

  #include <iostream>

  extern int yylex();
  extern int yyparse();
 
  void yyerror(const char *s);
%}

%union {
  int ival;
}

%token WHITESPACE
%token ENDL

%token LPAREN
%token RPAREN
%token SEMICOLON

%token ADD
%token SUB
%token MUL
%token DIV

%token PRINT
%token <ival> INT

%left ADD SUB
%left MUL DIV
%nonassoc UMINUS

%%

program: PRINT LPAREN exp RPAREN SEMICOLON endls
       ;

exp:
     INT
   | LPAREN exp RPAREN
   | SUB exp %prec UMINUS
   | exp ADD exp
   | exp SUB exp
   | exp MUL exp
   | exp DIV exp

endls:
     endls ENDL |
     ENDL
     ;

%%

void yyerror(const char *s) {
std::cout << s << '\n';
}
