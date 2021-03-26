#include "ast.h"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Value.h>

#include <memory>

namespace codegen {

struct ir_builder : llvm::IRBuilder<llvm::NoFolder> {
  using llvm::IRBuilder<llvm::NoFolder>::IRBuilder;
};

std::unique_ptr<llvm::Module> emit_llvm(ast::node const&, llvm::LLVMContext&);

namespace detail {

llvm::Function* main_decl(llvm::Module& mod);
llvm::Function* printf_decl(llvm::Module& mod);

} // namespace detail

} // namespace codegen
