#include <benchmark/benchmark.h>

#include "misc_util.hpp"
#include "oneflow/core/common/maybe.h"
#include "std_inc.hpp"

using namespace oneflow;

struct Test {
  Test(int x) : a(x) {}
  float a;
};

template <typename T>
shared_ptr<Test> imp_self_add(T *A, T *B, int N) {
  for (auto i = 0; i < N; i++) {
    A[i] = std::plus<T>()(A[i], B[i]);
    // A[i] += B[i];
  }
  return make_shared<Test>(A[N - 1]);
}

static void imp_self_add(benchmark::State &state) {
  int N = state.range();
  vector<float> A(N, 0);
  vector<float> B(N, 0);

  util::Random<float>(A.data(), A.size(), 0.123, 10.123, 0x12);
  util::Random<float>(B.data(), B.size(), 0.456, 10.456, 0x21);

  for (auto _ : state) {
    // state.PauseTiming();
    // state.ResumeTiming();
    benchmark::DoNotOptimize(A.data());
    benchmark::DoNotOptimize(B.data());
    imp_self_add(A.data(), B.data(), N);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(imp_self_add)->RangeMultiplier(2)->Range(1, 1 << 16);

template <typename T>
Maybe<Test> imp_std_plus(T *A, T *B, int N) {
  for (auto i = 0; i < N; i++) {
    A[i] = std::plus<T>()(A[i], B[i]);
  }
  return Test(A[N - 1]);
}

static void test_std_plus(benchmark::State &state) {
  int N = state.range();
  vector<float> A(N, 0);
  vector<float> B(N, 0);

  util::Random<float>(A.data(), A.size(), 0.123, 10.123, 0x12);
  util::Random<float>(B.data(), B.size(), 0.456, 10.456, 0x21);

  for (auto _ : state) {
    // state.PauseTiming();
    // state.ResumeTiming();
    benchmark::DoNotOptimize(A.data());
    benchmark::DoNotOptimize(B.data());
    imp_std_plus<float>(A.data(), B.data(), N);
    benchmark::ClobberMemory();
  }
}
BENCHMARK(test_std_plus)->RangeMultiplier(2)->Range(1, 1 << 16);

BENCHMARK_MAIN();
