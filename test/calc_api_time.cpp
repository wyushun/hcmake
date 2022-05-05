#include "npu_testing.h"

int main(int argc, char** argv) {
  auto npu = make_unique<NPUTesting>();
  npu->calc_api_time();
  return 0;
}
