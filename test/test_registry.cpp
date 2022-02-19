#include "registry.hpp"

int main(int argc, char **argv) {
  auto f = Registry::Global().GetCreator("Derived1");
  auto derived1 = f();
  derived1->foo();

  auto f2 = Registry::Global().GetCreator("Derived2");
  auto derived2 = f2();
  derived2->foo();
  return 0;
}
