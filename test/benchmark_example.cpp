#include <benchmark/benchmark.h>

#include "std_inc.hpp"

#define BENCHMARK_TEMPLATE_TEST(object)     \
  BENCHMARK_TEMPLATE(test_##object, 1);     \
  BENCHMARK_TEMPLATE(test_##object, 8);     \
  BENCHMARK_TEMPLATE(test_##object, 32);    \
  BENCHMARK_TEMPLATE(test_##object, 64);    \
  BENCHMARK_TEMPLATE(test_##object, 80);    \
  BENCHMARK_TEMPLATE(test_##object, 100);   \
  BENCHMARK_TEMPLATE(test_##object, 128);   \
  BENCHMARK_TEMPLATE(test_##object, 200);   \
  BENCHMARK_TEMPLATE(test_##object, 300);   \
  BENCHMARK_TEMPLATE(test_##object, 400);   \
  BENCHMARK_TEMPLATE(test_##object, 500);   \
  BENCHMARK_TEMPLATE(test_##object, 800);   \
  BENCHMARK_TEMPLATE(test_##object, 1000);  \
  BENCHMARK_TEMPLATE(test_##object, 1500);  \
  BENCHMARK_TEMPLATE(test_##object, 2000);  \
  BENCHMARK_TEMPLATE(test_##object, 2500);  \
  BENCHMARK_TEMPLATE(test_##object, 3000);  \
  BENCHMARK_TEMPLATE(test_##object, 4000);  \
  BENCHMARK_TEMPLATE(test_##object, 5000);  \
  BENCHMARK_TEMPLATE(test_##object, 6000);  \
  BENCHMARK_TEMPLATE(test_##object, 7000);  \
  BENCHMARK_TEMPLATE(test_##object, 8000);  \
  BENCHMARK_TEMPLATE(test_##object, 9000);  \
  BENCHMARK_TEMPLATE(test_##object, 10000); \
  BENCHMARK_TEMPLATE(test_##object, 20000); \
  BENCHMARK_TEMPLATE(test_##object, 30000); \
  BENCHMARK_TEMPLATE(test_##object, 40000); \
  BENCHMARK_TEMPLATE(test_##object, 50000); \
  BENCHMARK_TEMPLATE(test_##object, 60000); \
  BENCHMARK_TEMPLATE(test_##object, 70000); \
  BENCHMARK_TEMPLATE(test_##object, 80000); \
  BENCHMARK_TEMPLATE(test_##object, 90000); \
  BENCHMARK_TEMPLATE(test_##object, 100000);

// ----------------------------------------------------
template <uint32_t SIZE>
void test_shared_ptr(benchmark::State &state) {
  struct S {
    char arr[SIZE];
  };

  for (auto _ : state) {
    auto p = shared_ptr<S>(new S());
    benchmark::ClobberMemory();
  }
}
BENCHMARK_TEMPLATE_TEST(shared_ptr)

// ----------------------------------------------------
template <uint32_t SIZE>
void test_make_shared(benchmark::State &state) {
  struct S {
    char arr[SIZE];
  };

  for (auto _ : state) {
    auto p = make_shared<S>();
    benchmark::ClobberMemory();
  }
}
BENCHMARK_TEMPLATE_TEST(make_shared)

BENCHMARK_MAIN();
