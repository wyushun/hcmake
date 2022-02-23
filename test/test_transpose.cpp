#include "naive_tensor.hpp"
#include "std_inc.hpp"

int main(int argc, char **argv) {
  auto t = make_unique<NaiveTensor>(vector<int>{2, 10});
  t->Print();
  t->PrintData();

  t->NaiveTranspose(vector<int>{1, 0});
  t->PrintData();
  return 0;
}
