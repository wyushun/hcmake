#pragma once

#include "std_inc.hpp"

class Base;

using MyCreator = std::function<unique_ptr<Base>()>;
struct Registry {
  static Registry &Global() {
    static Registry r;
    return r;
  }

  Registry &Register(const string &name, MyCreator creator) {
    m_[name] = creator;
    return *this;
  }

  MyCreator GetCreator(const string& name) const {
    CHECK(m_.find(name) != m_.end());
    return m_.at(name);
  }

private:
  map<string, MyCreator> m_;
};

#define CONCAT_AB_(A, B) A##B
#define CONCAT_AB(A, B) CONCAT_AB_(A, B)
#define REGISTER_BASE_CREATOR(name)                                            \
  static Registry &CONCAT_AB(test_reg_, __COUNTER__) =                         \
      Registry::Global().Register(#name, []() { return std::make_unique<name>(); })
