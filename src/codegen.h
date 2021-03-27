#include "ast.h"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Value.h>

#include <memory>

namespace codegen {

/*
 * This isn't strictly necessary, but the generated code for this language isn't
 * very interesting to look at after it's been constant-folded away by the IR
 * builder.
 *
 * We expose this type as a derived class so that other modules can
 * forward-declare it (which isn't the case for a using-declaration).
 */
class ir_builder : public llvm::IRBuilder<llvm::NoFolder> {
  using llvm::IRBuilder<llvm::NoFolder>::IRBuilder;
};

/*
 * The public entrypoint for code generation - creates a new module in the given
 * context, acquires declarations for printf and main, then emits the generated
 * code from the AST into the body of main.
 */
std::unique_ptr<llvm::Module> emit_llvm(ast::node const&, llvm::LLVMContext&);

namespace detail {

/*
 * Hidden internal implementation details; these functions just construct the
 * statically known types for their function of interest, then look up or create
 * a declaration for it in the supplied module.
 */
llvm::Function* main_decl(llvm::Module& mod);
llvm::Function* printf_decl(llvm::Module& mod);

} // namespace detail

} // namespace codegen
