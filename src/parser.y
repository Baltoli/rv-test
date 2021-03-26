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
%parse-param { std::unique_ptr<ast::node>& node }

%code top {
  #include "../src/ast.h"
  #include "../src/lexer.h"
  #include "parser.hpp"

  #include <iostream>
  #include <memory>

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

%type <std::unique_ptr<ast::node>> exp

%%

program:
       PRINT LPAREN exp RPAREN SEMICOLON endls
       { node = std::move($3); }

exp:
       INT { $$ = std::make_unique<ast::constant>($1); }
     | LPAREN exp RPAREN { $$ = std::move($2); }
     | SUB exp %prec UMINUS 
        { 
          $$ = std::make_unique<ast::unary_op>(ast::unary_op_kind::minus, std::move($2));
        }
     | exp ADD exp
        {
          $$ = std::make_unique<ast::binary_op>(
            ast::binary_op_kind::add, std::move($1), std::move($3));
        }
     | exp SUB exp
        {
          $$ = std::make_unique<ast::binary_op>(
            ast::binary_op_kind::sub, std::move($1), std::move($3));
        }
     | exp MUL exp
        {
          $$ = std::make_unique<ast::binary_op>(
            ast::binary_op_kind::mul, std::move($1), std::move($3));
        }
     | exp DIV exp
        {
          $$ = std::make_unique<ast::binary_op>(
            ast::binary_op_kind::div, std::move($1), std::move($3));
        }

endls:
       ENDL endls
     | ENDL

%%

void yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << "Error: " << m << '\n';
}
