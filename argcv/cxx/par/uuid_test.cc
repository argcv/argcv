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
#include "argcv/cxx/par/uuid.h"

#include <iostream>

#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::Uuid;

TEST(Uuid, Compare) {
  Uuid u1;
  Uuid u2;
  EXPECT_TRUE(u1 <= u2);
  EXPECT_TRUE(u1 < u2);
  EXPECT_TRUE(u2 >= u1);
  EXPECT_TRUE(u2 > u1);
}

TEST(Uuid, Copy) {
  Uuid u1;
  Uuid u2 = u1;
  EXPECT_TRUE(u1 == u2);
  //
  // std::cout << "u1.hex: " << u1.Hex() << ":" << u1.Hex().size() << std::endl;
  // int ix = 0;
  // for(char c: u1.Hex()) {
  //   std::cout << "u1.hex:" << c << "/" << std::to_string(ix++)<< std::endl;
  // }
  // std::cout << "u1.str: " << u1.ToString() << ":" << u1.ToString().size() <<
  // std::endl;
  //
  // ix = 0;
  // for(char c: u1.ToString()) {
  //   std::cout << "u1.hex:" << c << "/" << std::to_string(ix++)<< std::endl;
  // }
  //
  // std::cout << "u1.pair (" << std::to_string(u1.Pair().first) << "," <<
  // std::to_string(u1.Pair().second) << ")"<<std::endl;

  Uuid u3(u1.data());
  EXPECT_TRUE(u1 == u3);

  // std::cout << "u3.pair (" << std::to_string(u3.Pair().first) << "," <<
  // std::to_string(u3.Pair().second) << ")"<<std::endl;

  Uuid u4(u1.Hex());
  EXPECT_TRUE(u1 == u4);

  // std::cout << "u4.pair (" << std::to_string(u4.Pair().first) << "," <<
  // std::to_string(u4.Pair().second) << ")"<<std::endl;

  Uuid u5(u1.ToString());
  EXPECT_TRUE(u1 == u5);

  // std::cout << "u5.pair (" << std::to_string(u5.Pair().first) << "," <<
  // std::to_string(u5.Pair().second) << ")"<<std::endl;

  std::pair<uint64_t, uint64_t> p = u1.Pair();
  Uuid u6(p.first, p.second);
  EXPECT_TRUE(u1 == u6);
}

TEST(Uuid, ToString) {
  Uuid u1;
  EXPECT_EQ(u1.data().size(), sizeof(uint64_t) * 2);  // 16
  EXPECT_EQ(u1.Hex().size(), 32);
  EXPECT_EQ(u1.ToString().size(), 36);
  LOG(INFO) << "Uuid: u1:" << u1.Hex();
  LOG(INFO) << "Uuid: u1:" << u1.ToString();
  // std::string s = u1.ToString();
  // for(int i = 0; i < s.size(); i++) {
  //   if(s[i] == '-') {
  //     std::cout << "XXX " << std::to_string(i) << std::endl;
  //   }
  // }
}
