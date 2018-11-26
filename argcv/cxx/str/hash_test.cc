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
#include "argcv/cxx/str/hash.h"

// #include <iostream>

#include "gtest/gtest.h"

class BlzHasherTest : public testing::Test {
 public:
  argcv::BlzHasher hasher_;

  virtual void SetUp() {
    // this->hasher_.hash("", 0);
  }

  virtual void TearDown() {}

  uint64_t Hash(const std::string &k, uint16_t offset = 0) {
    return hasher_.Hash(k, offset);
  }
};

TEST_F(BlzHasherTest, Equal) {
  const std::string s0 = "argcv-test-0";
  const std::string s1 = "argcv-test-0";
  EXPECT_EQ(s0, s1);
  EXPECT_EQ(Hash(s0, 0), Hash(s1, 0));
  EXPECT_EQ(Hash(s0, 1), Hash(s1, 1));
  EXPECT_EQ(Hash(s0, 2), Hash(s1, 2));
}

TEST_F(BlzHasherTest, NotEqual) {
  const std::string s0 = "argcv-test-0";
  const std::string s1 = "argcv-test-1";
  EXPECT_NE(s0, s1);
  EXPECT_TRUE((Hash(s0, 0) != Hash(s1, 0)) || (Hash(s0, 1) != Hash(s1, 1)) ||
              (Hash(s0, 2) != Hash(s1, 2)));
}
