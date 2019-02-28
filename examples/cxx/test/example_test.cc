// Copyright 2019 Yu Jing
#include "examples/cxx/test/example_test.h"

#include <cstdio>

#include <map>

#include "argcv/cxx/base/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

TEST(FactorialTest, First) { EXPECT_EQ(1, 1); }

TEST(FactorialTest, Second) { EXPECT_NE(1, std::max(1, 2)); }

TEST(FactorialTest, Third) { EXPECT_LT(std::min(1, 2), std::max(1, 2)); }

TEST(FactorialTest, Forth) { EXPECT_NEAR(3.14159, 3.1413, 0.001); }

class ExampleOfTestFTest : public testing::Test {
 private:
  class Agent {
   public:
    Agent(ExampleOfTestFTest &m_o, int m_k) : o_(m_o), k_(m_k) {}

    operator int() const {
      LOG(INFO) << "ExampleOfTestFTest::Agent::get : " << k_ << "<-"
                << o_.get(k_);
      return o_.get(k_);
    }

    void operator=(int v) {
      LOG(INFO) << "ExampleOfTestFTest::Agent::set : " << k_ << "->" << v;
      o_.set(k_, v);
    }

   private:
    ExampleOfTestFTest &o_;
    int k_;
  };

 public:
  int get(int k) const { return a_.at(k); }

  void set(int k, int v) {
    if (a_.find(k) == a_.end()) {
      a_.insert(std::pair<int, int>(k, v));
    } else {
      a_[k] = v;
    }
  }

  virtual void SetUp() {
    InitTestGoogleLogging();

    LOG(INFO) << "ExampleOfTestFTest::SetUp start";
    (*this)[1] = 2;
    (*this)[2] = 3;
    (*this)[3] = 4;
    LOG(INFO) << "ExampleOfTestFTest::SetUp end";
  }

  virtual void TearDown() {
    LOG(INFO) << "ExampleOfTestFTest::TearDown start";
    LOG(INFO) << "ExampleOfTestFTest::TearDown end";
  }

  Agent operator[](int k) { return Agent(*this, k); }

  size_t size() { return a_.size(); }

 private:
  std::map<int, int> a_;
};

TEST_F(ExampleOfTestFTest, oneToTwo) {
  EXPECT_EQ(2, (*this)[1]);
  (*this)[2] = 4;
  EXPECT_EQ(4, (*this)[2]);
}

TEST_F(ExampleOfTestFTest, twoToThree) {
  EXPECT_EQ(3, (*this)[2]);
  (*this)[4] = 7;
  EXPECT_EQ((size_t)4, size());
}

TEST_F(ExampleOfTestFTest, getSize) { EXPECT_EQ((size_t)3, size()); }
