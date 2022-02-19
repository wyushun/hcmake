#include "registry.hpp"
#include "derived1.hpp"

void Derived1::foo() { cout << "Derived1::foo()" << endl; }

REGISTER_BASE_CREATOR(Derived1);
