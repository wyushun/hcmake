#include "registry.hpp"

void Derived1::foo() { cout << "Derived1::foo()" << endl; }

void Derived2::foo() { cout << "Derived2::foo()" << endl; }

Registry &Registry::Global() {
  static Registry r;
  return r;
}

REGISTER_BASE_CREATOR(Derived1);
REGISTER_BASE_CREATOR(Derived2);
