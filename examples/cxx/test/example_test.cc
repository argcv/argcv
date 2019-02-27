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
#include "examples/cxx/test/example_test.h"

#include <cstdio>

#include <map>

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
      printf("ExampleOfTestFTest::Agent::get : %d <- %d\n", k_, o_.get(k_));
      return o_.get(k_);
    }

    void operator=(int v) {
      printf("ExampleOfTestFTest::Agent::set : %d -> %d\n", k_, v);
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
    printf("ExampleOfTestFTest::SetUp start\n");
    (*this)[1] = 2;
    (*this)[2] = 3;
    (*this)[3] = 4;
    printf("ExampleOfTestFTest::SetUp end\n");
  }

  virtual void TearDown() {
    printf("ExampleOfTestFTest::TearDown start\n");
    printf("ExampleOfTestFTest::TearDown end\n");
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
