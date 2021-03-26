#include "ast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.hpp"

#include <llvm/Support/raw_ostream.h>

#include <fstream>

int main(int argc, char** argv)
{
  if (argc < 2) {
    return 1;
  }

  auto in = std::ifstream(argv[1]);

  lexer lex(&in);
  std::unique_ptr<ast::node> root(nullptr);

  yy::parser p(lex, root);
  p.parse();

  root->dump();
  std::cout << '\n';

  llvm::LLVMContext ctx;
  auto mod = codegen::emit_llvm(*root, ctx);
  llvm::outs() << *mod << '\n';
}

