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

#include "argcv/cxx/env/macros.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

//

class CompariableInteger {
 public:
  explicit CompariableInteger(int value) : value_(value) {}

  const int Compare(const int& rhs) const noexcept {
    LOG(INFO) << "CALLED: [int Compare(const int& rhs)]";
    return value_ - rhs;
  }

  const int Compare(const CompariableInteger& rhs) const noexcept {
    LOG(INFO) << "CALLED: [int Compare(const CompariableInteger& rhs)]";
    return value_ - rhs.value_;
  }

  OP_COMPARABLE(int);
  OP_COMPARABLE(CompariableInteger);

 private:
  int value_;
};

TEST(Macros, OpComparableTypeEq) {
  CompariableInteger lhs(1);
  CompariableInteger rhs_sm(0);
  CompariableInteger rhs_eq(1);
  CompariableInteger rhs_gt(2);

  EXPECT_GT(lhs, rhs_sm);
  EXPECT_NE(lhs, rhs_sm);
  EXPECT_GE(lhs, rhs_eq);
  EXPECT_EQ(lhs, rhs_eq);
  EXPECT_LE(lhs, rhs_eq);
  EXPECT_LT(lhs, rhs_gt);
  EXPECT_NE(lhs, rhs_gt);

  EXPECT_TRUE(lhs > rhs_sm);
  EXPECT_TRUE(lhs != rhs_sm);
  EXPECT_TRUE(lhs >= rhs_eq);
  EXPECT_TRUE(lhs == rhs_eq);
  EXPECT_TRUE(lhs <= rhs_eq);
  EXPECT_TRUE(lhs < rhs_gt);
  EXPECT_TRUE(lhs != rhs_gt);

  EXPECT_FALSE(lhs < rhs_sm);
  EXPECT_FALSE(lhs <= rhs_sm);
  EXPECT_FALSE(lhs != rhs_eq);
  EXPECT_FALSE(lhs > rhs_gt);
}

TEST(Macros, OpComparableTypeNE) {
  CompariableInteger lhs(1);

  EXPECT_GT(lhs, 0);
  EXPECT_NE(lhs, 0);
  EXPECT_GE(lhs, 1);
  EXPECT_EQ(lhs, 1);
  EXPECT_LE(lhs, 1);
  EXPECT_LT(lhs, 2);
  EXPECT_NE(lhs, 2);

  // EXPECT_TRUE(lhs > 0);
  // EXPECT_TRUE(lhs != 0);
  // EXPECT_TRUE(lhs >= 1);
  // EXPECT_TRUE(lhs == 1);
  // EXPECT_TRUE(lhs <= 1);
  // EXPECT_TRUE(lhs < 2);
  // EXPECT_TRUE(lhs != 2);

  // EXPECT_FALSE(lhs < 0);
  // EXPECT_FALSE(lhs <= 0);
  // EXPECT_FALSE(lhs != 1);
  // EXPECT_FALSE(lhs > 2);
}
