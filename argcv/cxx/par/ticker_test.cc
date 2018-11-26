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
#include "argcv/cxx/par/ticker.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "argcv/cxx/par/duration.h"

using argcv::Duration;
using argcv::Ticker;

TEST(Ticker, Basic) {
  int c1 = 0;
  int c2 = 0;

  Ticker ticker;
  ticker.Sched(
      [&] {
        LOG(INFO) << "cnt:" << c1++;
        return c1 < 50;
      },
      Duration::NewMilliseconds(50));
  LOG(INFO) << "Sched::DONE :: c1";
  ticker.Sched(
      [&] {
        LOG(INFO) << "cnt2:" << c2++;
        return c2 < 100;
      },
      Duration::NewMilliseconds(25));
  LOG(INFO) << "Sched::DONE :: c2";
  ticker.Join();
  EXPECT_EQ(c1, 50);
  EXPECT_EQ(c2, 100);
}

TEST(Ticker, OutOfScope) {
  int c1 = 0;
  int c2 = 0;

  {
    Ticker ticker;
    ticker.Sched(
        [&] {
          LOG(INFO) << "cnt:" << c1++;
          return c1 < 50;
        },
        Duration::NewMilliseconds(50));
    LOG(INFO) << "Sched::DONE :: c1";
    ticker.Sched(
        [&] {
          LOG(INFO) << "cnt2:" << c2++;
          return c2 < 100;
        },
        Duration::NewMilliseconds(25));
    LOG(INFO) << "Sched::DONE :: c2";
    Duration::NewMilliseconds(500).Wait();
  }
  LOG(INFO) << "ALL READY, C1:" << c1 << ", C2:" << c2;
  EXPECT_LE(c1, 50);
  EXPECT_LE(c2, 100);
}
