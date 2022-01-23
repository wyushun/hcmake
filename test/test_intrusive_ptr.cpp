#include "intrusive_ptr.hpp"
#include "std_inc.hpp"

class Foo : public RefCount {
 public:
  Foo() { cout << "Foo ctor" << endl; }
  ~Foo() { cout << "Foo dtor" << endl; }

  void print_rc() { cout << cnt_ << endl; }
};

int main(int argc, char **argv) {
  intrusive_ptr<Foo> foo(new Foo());

  foo->print_rc();

  auto foo2 = foo;
  foo2->print_rc();

  auto foo3 = foo;
  foo3->print_rc();

  auto foo4 = std::move(foo);
  foo4->print_rc();

  return 0;
}
