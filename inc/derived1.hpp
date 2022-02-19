#pragma once

#include "base.hpp"

class Derived1 : public Base {
public:
  virtual void foo() override final;
};
