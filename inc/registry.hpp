#pragma once

#include "std_inc.hpp"

struct Base {
  virtual ~Base() = default;
  virtual void foo() = 0;
};

struct Derived1 : public Base {
  virtual void foo() override final;
};

struct Derived2 : public Base {
  virtual void foo() override final;
};

using MyCreator = std::function<unique_ptr<Base>()>;
struct Registry {
  static Registry &Global();
  Registry &Register(const string &name, MyCreator creator) {
    m_[name] = creator;
    return *this;
  }
private:
  map<string, MyCreator> m_;
};

#define CONCAT_AB_(A, B) A##B
#define CONCAT_AB(A, B) CONCAT_AB_(A, B)
#define REGISTER_BASE_CREATOR(name)                                            \
  static Registry &CONCAT_AB(test_reg_, __COUNTER__) =                         \
      Registry::Global().Register(#name, []() { return std::make_unique<name>(); })
