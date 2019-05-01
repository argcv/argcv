// Copyright 2019 Yu Jing
#include "argcv/cxx/base/macros.h"

#include "argcv/cxx/test/test.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

// class CompariableInteger {
//  public:
//   explicit CompariableInteger(int value) : value_(value) {}

//   const int Compare(const int& rhs) const noexcept {
//     LOG(INFO) << "CALLED: [int Compare(const int& rhs)]";
//     return value_ - rhs;
//   }

//   const int Compare(const CompariableInteger& rhs) const noexcept {
//     LOG(INFO) << "CALLED: [int Compare(const CompariableInteger& rhs)]";
//     return value_ - rhs.value_;
//   }

//   OP_COMPARABLE(int);
//   OP_COMPARABLE(CompariableInteger);

//  private:
//   int value_;
// };

class CompariableInteger : public Comparable<int>,
                           public Comparable<CompariableInteger> {
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

 private:
  int value_;
};

TEST(Macros, OpComparableTypeEq) {
  InitGoogleTestLogging();

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
  InitGoogleTestLogging();

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
