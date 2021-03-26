#include "ast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.hpp"

#include <llvm/Bitcode/BitcodeWriter.h>
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

  llvm::LLVMContext ctx;
  auto mod = codegen::emit_llvm(*root, ctx);

  std::error_code ec;
  llvm::raw_fd_ostream out_f("rv.bc", ec);

  llvm::WriteBitcodeToFile(*mod, out_f);
}

