#include "npu_testing.h"

int main(int argc, char** argv) {
  auto npu = make_unique<NPUTesting>();
  npu->alloc_mem();
  return 0;
}

