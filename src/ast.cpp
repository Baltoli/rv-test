#include "ast.h"
#include "codegen.h"

using namespace codegen;
using namespace llvm;

namespace ast {

Value* constant::emit(ir_builder& b) const { return b.getInt64(value); }

Value* unary_op::emit(ir_builder& b) const
{
  switch (kind) {
  case unary_op_kind::minus:
    return b.CreateNeg(exp->emit(b));
  default:
    assert(false && "Bad unary op kind");
  }
}

Value* binary_op::emit(ir_builder& b) const
{
  auto lhs = left->emit(b);
  auto rhs = right->emit(b);

  switch (kind) {
  case binary_op_kind::add:
    return b.CreateAdd(lhs, rhs);
  case binary_op_kind::sub:
    return b.CreateSub(lhs, rhs);
  case binary_op_kind::mul:
    return b.CreateMul(lhs, rhs);
  case binary_op_kind::div:
    return b.CreateSDiv(lhs, rhs);
  default:
    assert(false && "Bad binary op kind");
  }
}

} // namespace ast
