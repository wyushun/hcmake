#include "npu_testing.h"

int main(int argc, char** argv) {
  auto npu = make_unique<NPUTesting>();
  npu->read_regs();
  return 0;
}
