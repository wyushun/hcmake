#include "basis.hpp"

// clang-format off
const static array<uint64_t, E2I(MemUnit::NUM)> memUnitSize = {
  1 * 1024 * 1024 * 1024,
  1 * 1024 * 1024,
  1 * 1024,
  1,
};

const static array<string, E2I(MemUnit::NUM)> memUnitName {
  "G",
  "M",
  "K",
  "B",
};
// clang-format on

void timeBegin(struct timeval &begin) { gettimeofday(&begin, 0); }

unsigned long timeEnd(const struct timeval &begin, TimeUnit type) {
  struct timeval end;
  gettimeofday(&end, 0);
  if (type == TimeUnit::TIME_S) {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1000000;
  } else if (type == TimeUnit::TIME_MS) {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1000;
  } else {
    return (1000000 * (end.tv_sec - begin.tv_sec) +
            (end.tv_usec - begin.tv_usec)) /
           1;
  }
}

string showMemSize(uint64_t size) {
  if (size == 0) return "0B";

  std::ostringstream oss;

  for (auto i = 0; i < E2I(MemUnit::NUM); i++) {
    auto n = size / memUnitSize[i];
    size %= memUnitSize[i];
    if (n) {
      oss << n << memUnitName[i];
    }
  }

  return oss.str();
}
