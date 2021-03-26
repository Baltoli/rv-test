#include "ast.h"
#include "lexer.h"
#include "parser.hpp"

#include <fstream>

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }

  auto in = std::ifstream(argv[1]);

  lexer lex(&in);
  ast::node root;

  yy::parser p(lex, root);
  p.parse();
}

