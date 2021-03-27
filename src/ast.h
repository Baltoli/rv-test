#pragma once

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

#include <iostream>
#include <memory>

// Get access to the codegen module's IR builder type. This means that the
// actual concrete type can be changed in one place and this module can stay in
// sync (e.g. if we were to move back to the default, constant-folding builder).
namespace codegen {
struct ir_builder;
}

namespace ast {

/*
 * Represents the opcode of a unary operation (of which there is only one in
 * this language - unary negation).
 */
enum class unary_op_kind { minus };

/*
 * Represents the opcode of a binary operation per the language semantics (+, -,
 * * or /).
 */
enum class binary_op_kind { add, sub, mul, div };

/*
 * Abstract base class for Exp nodes in the language AST.
 *
 * The only operation provided by subclasses is to recursively emit LLVM IR to
 * an IR builder.
 */
class node {
public:
  /*
   * Abstracts away the instantiation of a lexer / parser pair from the
   * generated flex / bison code.
   */
  static std::unique_ptr<node> parse(std::istream&);

  /*
   * Produce an LLVM value representing the evaluation of this node as a
   * program.
   */
  virtual llvm::Value* emit(codegen::ir_builder&) const = 0;

  virtual ~node() = default;
};

/*
 * AST node representing a single constant 64-bit integer.
 */
class constant : public node {
public:
  constant(int64_t v)
      : value(v)
  {
  }

  /*
   * Emits an LLVM constant of type i64
   */
  llvm::Value* emit(codegen::ir_builder&) const override;

private:
  int64_t value;
};

/*
 * AST node representing a unary operation applied to a nested expression node.
 */
class unary_op : public node {
public:
  unary_op(unary_op_kind k, std::unique_ptr<ast::node> e)
      : kind(k)
      , exp(std::move(e))
  {
  }

  /*
   * Emits code for the nested expression, then emits code to apply the
   * appropriate unary operation to the result.
   */
  llvm::Value* emit(codegen::ir_builder&) const override;

private:
  unary_op_kind kind;
  std::unique_ptr<ast::node> exp;
};

/*
 * AST node representing a binary operation applied to two nested expressions
 * (the left and right-hand sides).
 */
class binary_op : public node {
public:
  binary_op(
      binary_op_kind k, std::unique_ptr<ast::node> l,
      std::unique_ptr<ast::node> r)
      : kind(k)
      , left(std::move(l))
      , right(std::move(r))
  {
  }

  /*
   * Emit code for the LHS, then the RHS nested expression, then apply the
   * appropriate operation to those results.
   */
  llvm::Value* emit(codegen::ir_builder&) const override;

private:
  binary_op_kind kind;
  std::unique_ptr<ast::node> left;
  std::unique_ptr<ast::node> right;
};

} // namespace ast
