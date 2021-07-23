// Copyright 2021 Yu Jing
#include "argcv/sync/wait_group.h"

#include <cstdio>
#include <vector>

#include "argcv/base/thread_pool.h"
#include "argcv/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {
// using argcv::ThreadPool;
using ::argcv::sync::WaitGroup;

TEST(WaitGroup, WaitGroup) {
  size_t thread_size = 3;
  size_t task_size = 100;
  ThreadPool pool(thread_size);

  uint32 gt_score = 0;
  atomic<uint32> score(0);

  WaitGroup wg;
  for (size_t i = 0; i < task_size; i++) {
    wg.Add();
    gt_score += static_cast<uint32>(i);
    pool.Enqueue([i, &wg, &score] {
      usleep(20);
      AtomicFetchAdd<uint32>(&score, static_cast<uint32>(i));
      wg.Done();
    });
  }
  wg.Wait();

  EXPECT_EQ(gt_score, score.load());
}

}  // namespace argcv
