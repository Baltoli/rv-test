#include "parser.hpp"

#include <cstdio>

extern FILE* yyin;

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }

  auto input_file = fopen(argv[1], "r");
  if (!input_file) {
    return 2;
  }

  yyin = input_file;
  yyparse();

  fclose(input_file);
}

