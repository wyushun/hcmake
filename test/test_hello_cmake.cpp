#include <benchmark/benchmark.h>

#include "oneflow/core/common/maybe.h"
#include "std_inc.hpp"

using namespace oneflow;

struct Foo {
  Foo() : a(0), b(1), c(-100) {}
  int a;
  int b;
  int c;
};
Foo foo;

// ----------------------------------------------------
int test_norm_void(Foo &foo, int i) {
  foo.a = i;
  foo.b = i + 1;
  foo.c = i + 2;
  return foo.a + 3;
}
void test_bench_void(benchmark::State &state) {
  auto i = 0;
  for (auto _ : state) {
    benchmark::DoNotOptimize(&foo);
    benchmark::DoNotOptimize(test_norm_void);
    auto ret = test_norm_void(foo, i++);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(test_bench_void);

// ----------------------------------------------------
Maybe<int> test_maybe_void(Foo &foo, int i) {
  foo.a = i;
  foo.b = i + 2;
  foo.c = i + 3;
  return foo.b + 100;
}
void test_bench_maybe(benchmark::State &state) {
  auto i = 0;
  for (auto _ : state) {
    benchmark::DoNotOptimize(&foo);
    benchmark::DoNotOptimize(test_maybe_void);
    auto ret = test_maybe_void(foo, i++);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(test_bench_maybe);

BENCHMARK_MAIN();
