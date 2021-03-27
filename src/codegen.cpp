#include "codegen.h"

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>

using namespace llvm;

namespace codegen {

using namespace detail;

std::unique_ptr<Module> emit_llvm(ast::node const& root, LLVMContext& ctx)
{
  auto mod = std::make_unique<Module>("rvc", ctx);

  auto printf_fn = printf_decl(*mod);
  auto main_fn = main_decl(*mod);

  auto entry = BasicBlock::Create(ctx, "entry", main_fn);

  ir_builder build(entry);

  // Fixed format string passed to printf, as we know the argument type is
  // always an i64.
  auto format = build.CreateGlobalStringPtr("%ld\n", "format");

  // The program output is the result of evaluating the AST root node.
  auto output = root.emit(build);

  build.CreateCall(printf_fn, {format, output});

  build.CreateRet(build.getInt32(0));

  return mod;
}

namespace detail {

Function* main_decl(Module& mod)
{
  auto& ctx = mod.getContext();

  // main has type int(int, char**)
  auto int_ty = IntegerType::get(ctx, 32);
  auto args_ty = IntegerType::get(ctx, 8)->getPointerTo()->getPointerTo();

  auto fn_ty = FunctionType::get(int_ty, {int_ty, args_ty}, false);

  return dyn_cast<Function>(mod.getOrInsertFunction("main", fn_ty).getCallee());
}

Function* printf_decl(Module& mod)
{
  auto& ctx = mod.getContext();

  // printf has type int(char*, ...)
  auto int_ty = IntegerType::get(ctx, 32);
  auto c_str_ty = IntegerType::get(ctx, 8)->getPointerTo();

  auto fn_ty = FunctionType::get(int_ty, {c_str_ty}, true);

  return dyn_cast<Function>(
      mod.getOrInsertFunction("printf", fn_ty).getCallee());
}

} // namespace detail

} // namespace codegen
