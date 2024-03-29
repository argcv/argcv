// Copyright 2021 Yu Jing
#include "argcv/base/concurrency.h"

#include "argcv/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {

TEST(Concurrency, Timing) {
  InitGoogleTestLogging();
  uint64 time_in = GetCurrentNanoseconds();
  Sleep(1200, 100);
  uint64 time_out = GetCurrentNanoseconds();
  uint64 gap = time_out - time_in;
  uint64 expect_gap = 1200 * 1000000L + 100;

  EXPECT_GT(gap, expect_gap * 0.8);
  EXPECT_LT(gap, expect_gap * 1.2);

  LOG(INFO) << "gap:" << gap << " expected: " << expect_gap;
}

}  // namespace argcv
