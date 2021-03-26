#pragma once

#include <iostream>
#include <memory>

namespace ast {

enum class unary_op_kind { minus };
enum class binary_op_kind { add, sub, mul, div };

inline char binary_op_char(binary_op_kind k)
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
  }
}

struct node {
  virtual void dump() const = 0;
  virtual ~node() = default;
};

struct constant : public node {
  constant(int64_t v)
      : value(v)
  {
  }

  void dump() const override { std::cout << value; }

  int64_t value;
};

struct unary_op : public node {
  unary_op(unary_op_kind k, std::unique_ptr<ast::node> e)
      : kind(k)
      , exp(std::move(e))
  {
  }

  void dump() const override
  {
    std::cout << "-(";
    exp->dump();
    std::cout << ")";
  }

  unary_op_kind kind;
  std::unique_ptr<ast::node> exp;
};

struct binary_op : public node {
  binary_op(
      binary_op_kind k, std::unique_ptr<ast::node> l,
      std::unique_ptr<ast::node> r)
      : kind(k)
      , left(std::move(l))
      , right(std::move(r))
  {
  }

  void dump() const override
  {
    std::cout << '(';
    left->dump();
    std::cout << ' ' << binary_op_char(kind) << ' ';
    right->dump();
    std::cout << ')';
  }

  binary_op_kind kind;
  std::unique_ptr<ast::node> left;
  std::unique_ptr<ast::node> right;
};

} // namespace ast
