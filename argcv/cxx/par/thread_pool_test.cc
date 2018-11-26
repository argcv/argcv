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
#include "argcv/cxx/par/thread_pool.h"

#include <cstdio>

#include <functional>
#include <future>  // NOLINT(build/c++11)
#include <vector>

#include "argcv/cxx/par/atomic.h"
#include "argcv/cxx/test/gtest_ext.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(ThreadPool, PlusOne) {
  atomic<uint32_t> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, thread_size, task_size, &apple] {
      LOG_EVERY_N(INFO, 20) << "[test_case_threads] start: " << i << " of ["
                            << thread_size << " @ " << task_size << "]";
      usleep(20);
      uint32_t res = AtomicFetchAdd<uint32_t>(&apple, (uint32_t)1);
      LOG_EVERY_N(INFO, 20) << "[test_case_threads] end: " << i << " of ["
                            << thread_size << " @ " << task_size << "]";
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple.load());
}

TEST(ThreadPool, PlusOneLimited) {
  atomic<uint32_t> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size, 4);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, thread_size, task_size, &apple] {
      LOG_EVERY_N(INFO, 1) << "[test_case_threads] start: " << i << " of ["
                           << thread_size << " @ " << task_size << "]";
      usleep(200);
      uint32_t res = AtomicFetchAdd<uint32_t>(&apple, (uint32_t)1);
      LOG_EVERY_N(INFO, 1) << "[test_case_threads] end: " << i << " of ["
                           << thread_size << " @ " << task_size << "]";
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple.load());
}
