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
#include "argcv/cxx/par/duration.h"
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>

#include "argcv/cxx/env/config.h"
#include "argcv/cxx/env/env.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::Duration;
using argcv::Env;

TEST(Duration, SpecialCase) {
  Duration d0 = Duration::Zero();
  EXPECT_EQ(d0.IsZero(), true);
  EXPECT_EQ(d0.IsValid(), true);
  Duration d1 = Duration::Invalid();
  EXPECT_EQ(d1.IsValid(), false);
}

TEST(Duration, Compare) {
  Duration d0 = Duration::NewNanoseconds(1000);
  Duration d1 = Duration::NewMicroseconds(1);
  EXPECT_EQ(d0, d1);
  Duration d2 = Duration::NewNanoseconds(999);
  Duration d3 = Duration::NewNanoseconds(1001);
  EXPECT_LT(d2, d3);
  EXPECT_LT(d1, d3);
  EXPECT_GT(d1, d2);
  EXPECT_GT(d1, d2);
}

TEST(Duration, Wait) {
  Env *e = Env::Default();
  uint64_t t1 = e->NowNanoseconds();
  Duration::NewMilliseconds(120).Wait();
  uint64_t t2 = e->NowNanoseconds();
  uint64_t ns_in_100ms = 100000000L;
  uint64_t ns_in_150ms = 150000000L;
  EXPECT_LT(ns_in_100ms, t2 - t1);
  EXPECT_GT(ns_in_150ms, t2 - t1);
}

TEST(Duration, Calculate) {
  Duration d0 = Duration::NewSeconds(1);
  EXPECT_EQ(d0, Duration::NewMilliseconds(1000));
  Duration d1 = Duration::NewMilliseconds(600);
  EXPECT_EQ(d1, Duration::NewMilliseconds(600));
  Duration d2 = d0 - d1;
  EXPECT_EQ(d2, Duration::NewMilliseconds(400));
  Duration d3 = d0 + d1;
  EXPECT_EQ(d3, Duration::NewMilliseconds(1600));
  d0 -= d1;
  EXPECT_EQ(d0, Duration::NewMilliseconds(400));
  d1 += d0;
  EXPECT_EQ(d1, Duration::NewMilliseconds(1000));
  Duration d4 = d0;
  EXPECT_EQ(d4, Duration::NewMilliseconds(400));
}
