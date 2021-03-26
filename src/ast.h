#pragma once

#include <iostream>

namespace ast {

class node {
public:
  virtual void dump() const { std::cout << "node\n"; }

private:
};

class constant : public node {
public:
  constant(int64_t v)
      : value(v)
  {
  }

  void dump() const override { std::cout << "const(" << value << ")\n"; }

  int64_t value;

private:
};

} // namespace ast
