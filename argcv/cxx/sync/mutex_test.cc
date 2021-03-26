// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/mutex.h"

#include <cstdio>
#include <vector>

#include "argcv/cxx/base/thread_pool.h"
#include "argcv/cxx/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::ThreadPool;
using argcv::sync::LockGuard;
using argcv::sync::Mutex;

TEST(Mutex, Mutex) {
  Mutex sl;
  sl.Lock();
  sl.Unlock();
  sl.Lock();
  sl.Unlock();
}

// Try add param 'max_parallelism' to make it passed in my linux sys
TEST(Mutex, MultipleThread) {
  uint32_t apple = 0;
  Mutex lock;
  size_t thread_size = 5;
  size_t max_parallelism = 20;
  size_t task_size = 10000;
  ThreadPool pool(thread_size, max_parallelism);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, &apple, &lock] {
      Mutex &my_lock = lock;
      usleep(i % 20);
      my_lock.Lock();
      uint32_t res = apple++;
      my_lock.Unlock();
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple);
}

// Try add param 'max_parallelism' to make it passed in my linux sys
TEST(Mutex, Guard) {
  uint32_t apple = 0;
  Mutex lock;
  size_t thread_size = 5;
  size_t max_parallelism = 20;
  size_t task_size = 10000;
  ThreadPool pool(thread_size, max_parallelism);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, &apple, &lock] {
      usleep(i % 20);
      auto g = lock.Guard();
      return apple++;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple);
}

TEST(LockGuard, MultipleThread) {
  uint32_t apple = 0;
  Mutex lock;
  size_t thread_size = 5;
  size_t max_parallelism = 20;
  size_t task_size = 10000;
  ThreadPool pool(thread_size, max_parallelism);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([&apple, &lock] {
      LockGuard guard = lock.Guard();
      uint32_t res = apple++;
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple);
}
