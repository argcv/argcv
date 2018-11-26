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
#include "argcv/cxx/par/atomic.h"

#include <cstdio>
#include <vector>

#include "argcv/cxx/par/thread_pool.h"
#include "argcv/cxx/test/gtest_ext.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace argcv;  // NOLINT(build/namespaces)

TEST(AtomicFetchAdd, PlusOne) {
  atomic<uint32_t> apple(0);
  EXPECT_EQ(0, AtomicFetchAdd<uint32_t>(&apple, 1));
  EXPECT_EQ(1, AtomicFetchAdd<uint32_t>(&apple, 1));
  EXPECT_EQ(2, AtomicFetchAdd<uint32_t>(&apple, 1));
  printf("score: %u\n", AtomicFetchAdd<uint32_t>(&apple, 1));
}

TEST(SharedAtomic, PlusOne) {
  SharedAtomic<uint32_t> apple(0);
  SharedAtomic<uint32_t> banana = apple;
  EXPECT_EQ(0, apple.FetchAdd(1));
  EXPECT_EQ(1, banana.FetchAdd(1));
  EXPECT_EQ(2, apple.FetchAdd(1));
  EXPECT_EQ(4, apple.SyncAdd(1));
  EXPECT_EQ(3, apple.SyncSub(1));
  SharedAtomic<uint32_t> grape;
  grape = apple;
  grape.Store(-1);
  EXPECT_EQ(-1, banana.Load());
  printf("score: %u\n", apple.FetchAdd(1));
}

TEST(SharedAtomic, MultipleThread) {
  SharedAtomic<uint32_t> apple(0);
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size);

  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, apple] {
      SharedAtomic<uint32_t> banana = apple;
      usleep(i % 20);
      uint32_t res = banana.FetchAdd(1);
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto &&result : results) {
    score += static_cast<uint32_t>(result.get());
  }
  EXPECT_EQ(gt_score, score);
  EXPECT_EQ((uint32_t)task_size, apple.Load());
}
