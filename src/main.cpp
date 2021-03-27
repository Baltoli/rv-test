#include "ast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.hpp"

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/raw_ostream.h>

#include <fstream>

/*
 * All the compiler entry point needs to do is accept a single command line
 * argument, then dispatch that input to the two phases of the compiler (parser
 * and code generation) in turn.
 *
 * There's no real error handling done here; if any of the phases fails then we
 * just print a message to stderr and exit with a nonzero return code.
 *
 * If compilation succeeds, the resulting bitcode will always get written to a
 * file named rv.bc in the current directory.
 */
int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <file>\n";
    return 1;
  }

  auto in = std::ifstream(argv[1]);
  auto&& root = ast::node::parse(in);
  if (!root) {
    std::cerr << "Parse error!\n";
    return 2;
  }

  llvm::LLVMContext ctx;
  auto mod = codegen::emit_llvm(*root, ctx);
  if (!mod) {
    std::cerr << "Codegen error!\n";
    return 3;
  }

  std::error_code ec;
  llvm::raw_fd_ostream out_f("rv.bc", ec);

  llvm::WriteBitcodeToFile(*mod, out_f);
}

