#include "pybind11/embed.h"

namespace py_exports {

namespace {
int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div(int a, int b) { return a / b; }
}  // namespace

PYBIND11_MODULE(example, m) {
  m.doc() = "math interfaces";
  m.def("add", &add, "do add");
  m.def("sub", &sub, "do sub");
  m.def("mul", &mul, "do mul");
  // m.def("div", &div, "do div");
}

}  // namespace py_exports
