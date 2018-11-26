/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include "argcv/cxx/par/lock.h"

#include <cstdio>
#include <vector>

#include "argcv/cxx/par/thread_pool.h"
#include "argcv/cxx/test/gtest_ext.h"
#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(Spinlock, Basic) {
  Spinlock sl;
  ASSERT_USECS(100, sl.Lock());
  ASSERT_USECS(100, sl.Unlock());
  ASSERT_USECS(100, sl.Lock());
  ASSERT_USECS(100, sl.Unlock());
}

// Try add param 'max_parallelism' to make it passed in my linux sys
TEST(Spinlock, MultipleThread) {
  uint32_t apple = 0;
  Spinlock lock;
  size_t thread_size = 5;
  size_t max_parallelism = 20;
  size_t task_size = 10000;
  ThreadPool pool(thread_size, max_parallelism);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, &apple, lock] {
      Spinlock my_lock = lock;
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

TEST(TicketLock, Basic) {
  TicketLock tl;
  ASSERT_USECS(100, tl.Lock());
  ASSERT_USECS(100, tl.Unlock());
  ASSERT_USECS(100, tl.Lock());
  ASSERT_USECS(100, tl.Unlock());
}

// TODO(yu): error found here some times
// Environment: Darwin im 17.5.0 Darwin Kernel Version 17.5.0
// Message:
// [ RUN      ] TicketLock.multiple_thread
// libc++abi.dylib: terminating with uncaught exception of type
// std::__1::system_error: thread constructor failed: Resource temporarily
// unavailable Abort trap: 6
// TODO(yu): Spent too much time in TicketLock
// [ RUN      ] TicketLock.multiple_thread
// [       OK ] TicketLock.multiple_thread (119050 ms) !!!?
// Try add param 'max_parallelism' to make it passed in my linux sys
TEST(TicketLock, MultipleThread) {
  uint32_t apple = 0;
  TicketLock lock;
  size_t thread_size = 5;
  size_t max_parallelism = 20;
  size_t task_size = 10000;
  ThreadPool pool(thread_size, max_parallelism);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, &apple, lock] {
      TicketLock my_lock = lock;
      // What's wrong here?
      // usleep(i % 20);
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
TEST(TicketLock, Guard) {
  uint32_t apple = 0;
  TicketLock lock;
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
