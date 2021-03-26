// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/once.h"

#include <cstdio>
#include <vector>

#include "argcv/cxx/base/thread_pool.h"
#include "argcv/cxx/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::sync::Once;

TEST(Once, Once) {
  Once once;
  int a = 1;
  int b = 2;
  int c = 0;
  for (int i = 0; i < 10; i++) {
    once.Do([a, b, &c](int v) { c += a + b + v; }, i);
  }
  EXPECT_EQ(3, c);
  LOG(INFO) << "a :" << a << ", b :" << b << ", c :" << c;
}
