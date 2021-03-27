#include "ast.h"
#include "codegen.h"
#include "lexer.h"
#include "parser.hpp"

using namespace codegen;
using namespace llvm;

namespace ast {

char binary_op_char(binary_op_kind k)
{
  switch (k) {
  case binary_op_kind::add:
    return '+';
  case binary_op_kind::sub:
    return '-';
  case binary_op_kind::mul:
    return '*';
  case binary_op_kind::div:
    return '/';
  default:
    assert(false && "Bad binary op kind");
  }
}

std::unique_ptr<node> node::parse(std::istream& in)
{
  lexer lex(&in);
  std::unique_ptr<ast::node> root(nullptr);

  yy::parser p(lex, root);
  auto ret = p.parse();
  if (ret != 0) {
    return nullptr;
  }

  return root;
}

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
