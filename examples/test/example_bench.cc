// Copyright 2021 Yu Jing
#include <cstring>
#include <string>

#include "benchmark/benchmark.h"

// // Dummy Example
// void BM_empty(benchmark::State& state) { // NOLINT non-const reference
//   for (auto _ : state) {
//     benchmark::DoNotOptimize(state.iterations());
//   }
// }
// // Register the function as a benchmark
// BENCHMARK(BM_empty);

// Define another benchmark
// Basic usage
// https://github.com/google/benchmark#basic-usage
static void BM_StringCreation(
    benchmark::State& state) {  // NOLINT non-const reference
  for (auto _ : state) std::string empty_string;
}

BENCHMARK(BM_StringCreation);

// Once More
static void BM_StringCopy(
    benchmark::State& state) {  // NOLINT non-const reference
  std::string x = "hello";
  for (auto _ : state) std::string copy(x);
}
BENCHMARK(BM_StringCopy);

// Passing arguments
// https://github.com/google/benchmark#passing-arguments
static void BM_memcpy(benchmark::State& state) {  // NOLINT non-const reference
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state) memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_memcpy)->Arg(8)->Arg(64);
// BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
// BENCHMARK(BM_memcpy)->Range(8, 8 << 10);
// BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(8, 8 << 10);

// static void BM_SetInsert(benchmark::State& state) { // NOLINT non-const
// reference
//   std::set<int> data;
//   for (auto _ : state) {
//     state.PauseTiming();
//     data = ConstructRandomSet(state.range(0));
//     state.ResumeTiming();
//     for (int j = 0; j < state.range(1); ++j)
//       data.insert(RandomNumber());
//   }
// }
// BENCHMARK(BM_SetInsert)
//     ->Args({1<<10, 128})
//     ->Args({2<<10, 128})
//     ->Args({4<<10, 128})
//     ->Args({8<<10, 128})
//     ->Args({1<<10, 512})
//     ->Args({2<<10, 512})
//     ->Args({4<<10, 512})
//     ->Args({8<<10, 512});
//
// BENCHMARK(BM_SetInsert)->Ranges({{1<<10, 8<<10}, {128, 512}});

// static void CustomArguments(benchmark::internal::Benchmark* b) {
//   for (int i = 0; i <= 10; ++i)
//     for (int j = 32; j <= 1024*1024; j *= 8)
//       b->Args({i, j});
// }
// BENCHMARK(BM_SetInsert)->Apply(CustomArguments);

// Calculate asymptotic complexity (Big O)
// https://github.com/google/benchmark#calculate-asymptotic-complexity-big-o

static void BM_StringCompare(
    benchmark::State& state) {  // NOLINT non-const reference
  std::string s1(state.range(0), '-');
  std::string s2(state.range(0), '-');
  for (auto _ : state) {
    benchmark::DoNotOptimize(s1.compare(s2));
  }
  state.SetComplexityN(state.range(0));
}

BENCHMARK(BM_StringCompare)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 12)
    ->Complexity(benchmark::oN);

BENCHMARK(BM_StringCompare)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 12)
    ->Complexity();

// BENCHMARK(BM_StringCompare)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 18)
//     ->Complexity(benchmark::oN);

// BENCHMARK(BM_StringCompare)
//     ->RangeMultiplier(2)
//     ->Range(1 << 10, 1 << 18)
//     ->Complexity();

// BENCHMARK(BM_StringCompare)->RangeMultiplier(2)
//     ->Range(1<<10, 1<<18)->Complexity([](int n)->double{return n; });

// Templated benchmarks
// https://github.com/google/benchmark#templated-benchmarks
// template <class Q> int BM_Sequential(benchmark::State& state) { // NOLINT
// non-const reference
//   Q q;
//   typename Q::value_type v;
//   for (auto _ : state) {
//     for (int i = state.range(0); i--; )
//       q.push(v);
//     for (int e = state.range(0); e--; )
//       q.Wait(&v);
//   }
//   // actually messages, not bytes:
//   state.SetBytesProcessed(
//       static_cast<int64_t>(state.iterations())*state.range(0));
// }
// BENCHMARK_TEMPLATE(BM_Sequential, WaitQueue<int>)->Range(1 << 0, 1 << 10);

// Multithreaded benchmarks
// https://github.com/google/benchmark#multithreaded-benchmarks
static void BM_MultiThreaded(
    benchmark::State& state) {  // NOLINT non-const reference
  if (state.thread_index == 0) {
    // Setup code here.
  }
  for (auto _ : state) {
    // Run the test as normal.
  }
  if (state.thread_index == 0) {
    // Teardown code here.
  }
}
BENCHMARK(BM_MultiThreaded)->Threads(2);
