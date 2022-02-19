#include "registry.hpp"
#include "derived2.hpp"

void Derived2::foo() { cout << "Derived2::foo()" << endl; }

REGISTER_BASE_CREATOR(Derived2);
