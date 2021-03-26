%skeleton "lalr1.cc"
%require "3.0.2"

%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert true

%code requires {
  #include "../src/ast.h"

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
%parse-param { ast::node &node }

%code top {
  #include "../src/ast.h"
  #include "../src/lexer.h"
  #include "parser.hpp"

  #include <iostream>

  static yy::parser::symbol_type yylex(lexer &scanner) {
    return scanner.get_next_token();
  }
}

%token PRINT
%token <int64_t> INT

%token LPAREN
%token RPAREN
%token SEMICOLON

%token ADD
%token SUB
%token MUL
%token DIV

%token END 0
%token ENDL

%left ADD SUB
%left MUL DIV
%nonassoc UMINUS

%type <ast::node*> exp

%%

program:
       PRINT LPAREN exp RPAREN SEMICOLON endls
       { $3->dump(); }

exp:
       INT { $$ = new ast::constant($1); }
     | LPAREN exp RPAREN { $$ = $2; }
     /* | SUB exp %prec UMINUS */
     /* | exp ADD exp */
     /* | exp SUB exp */
     /* | exp MUL exp */
     /* | exp DIV exp */

endls:
       ENDL endls
     | ENDL

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << "Error: " << m << '\n';
}
