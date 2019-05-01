// Copyright 2019 Yu Jing
#include "argcv/cxx/base/thread_pool.h"

#include <cstdio>

#include <functional>
#include <future>  // NOLINT(build/c++11)
#include <vector>

#include "argcv/cxx/base/atomic.h"
#include "argcv/cxx/base/types.h"
#include "argcv/cxx/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(ThreadPool, ThreadPool) {
  atomic<uint32> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size);

  std::vector<std::future<uint32>> results;
  uint32 gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += static_cast<uint32>(i);
    results.emplace_back(pool.Enqueue([i, thread_size, task_size, &apple] {
      LOG_EVERY_N(INFO, 20) << "[test_case_threads] start: " << i << " of ["
                            << thread_size << " @ " << task_size << "]";
      usleep(20);
      uint32 res = AtomicFetchAdd<uint32>(&apple, static_cast<uint32>(1));
      LOG_EVERY_N(INFO, 20) << "[test_case_threads] end: " << i << " of ["
                            << thread_size << " @ " << task_size << "]";
      return res;
    }));
  }

  uint32 score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32)task_size, apple.load());
}

TEST(ThreadPool, PlusOneLimited) {
  atomic<uint32> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size, 4);

  std::vector<std::future<uint32>> results;
  uint32 gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += static_cast<uint32>(i);
    results.emplace_back(pool.Enqueue([i, thread_size, task_size, &apple] {
      LOG_EVERY_N(INFO, 1) << "[test_case_threads] start: " << i << " of ["
                           << thread_size << " @ " << task_size << "]";
      usleep(200);
      uint32 res = AtomicFetchAdd<uint32>(&apple, static_cast<uint32>(1));
      LOG_EVERY_N(INFO, 1) << "[test_case_threads] end: " << i << " of ["
                           << thread_size << " @ " << task_size << "]";
      return res;
    }));
  }

  uint32 score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32)task_size, apple.load());
}
