// Copyright 2019 Yu Jing
#include "argcv/cxx/base/concurrency.h"

#include "argcv/cxx/test/test.h"

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

  EXPECT_GT(gap, expect_gap);
  EXPECT_LT(gap, expect_gap * 1.2);

  LOG(INFO) << "gap:" << gap << " expected: " << expect_gap;
}

}  // namespace argcv
