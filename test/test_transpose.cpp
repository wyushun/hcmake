#include "std_inc.hpp"
#include "tensor.hpp"

int main(int argc, char **argv) {
  auto t = make_unique<Tensor>(vector<int>{2, 10});
  t->print();
  t->print_data();
  return 0;
}
