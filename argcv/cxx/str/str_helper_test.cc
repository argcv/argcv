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
#include "argcv/cxx/str/str_helper.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <map>
#include <string>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {

// std::vector<std::string> Split(const std::string &s, const std::string
// &delim,
//                                bool skip_blank_elem = true) noexcept;

TEST(StrHelper, Split) {
  std::string s("aaa,bbb,ccc,");
  std::vector<std::string> v0 = Split(s, ",", true);
  EXPECT_EQ(3, v0.size());
  EXPECT_EQ("aaa", v0[0]);
  EXPECT_EQ("bbb", v0[1]);
  EXPECT_EQ("ccc", v0[2]);
  std::vector<std::string> v1 = Split(s, ",", false);
  EXPECT_EQ(4, v1.size());
  EXPECT_EQ("", v1[3]);
}

TEST(StrHelper, Replace) {
  std::string s("aaa,bbb,ccc,bbb,");
  std::string r0 = ReplaceAll(s, "bbb", "ddd");
  EXPECT_EQ("aaa,bbb,ccc,bbb,", s);
  EXPECT_EQ("aaa,ddd,ccc,ddd,", r0);
  std::string r1 = ReplaceAllInPlace(&s, "bbb", "ddd");
  EXPECT_EQ("aaa,ddd,ccc,ddd,", s);
  EXPECT_EQ("aaa,ddd,ccc,ddd,", r1);
}

TEST(StrHelper, Utf8Split) {
  std::string s("a,你好");
  std::vector<std::string> v0 = Utf8Split(s);
  EXPECT_EQ(4, v0.size());
  EXPECT_EQ("a", v0[0]);
  EXPECT_EQ(",", v0[1]);
  EXPECT_EQ("你", v0[2]);
  EXPECT_EQ("好", v0[3]);
}

TEST(StrHelper, Utf8ToUnicode) {
  std::string s("Abc123我爱你中文");
  std::vector<uint64_t> v0 = Utf8ToUnicode(s);
  EXPECT_EQ(11, v0.size());
  EXPECT_EQ(0x41, v0[0]);     // A
  EXPECT_EQ(0x62, v0[1]);     // b
  EXPECT_EQ(0x63, v0[2]);     // c
  EXPECT_EQ(0x31, v0[3]);     // 1
  EXPECT_EQ(0x32, v0[4]);     // 2
  EXPECT_EQ(0x33, v0[5]);     // 3
  EXPECT_EQ(0x6211, v0[6]);   // 我
  EXPECT_EQ(0x7231, v0[7]);   // 爱
  EXPECT_EQ(0x4F60, v0[8]);   // 你
  EXPECT_EQ(0x4E2D, v0[9]);   // 中
  EXPECT_EQ(0x6587, v0[10]);  // 文
}

TEST(StrHelper, Trim) {
  std::string s("\tabc\n ");
  std::string r0 = Trim(s);
  EXPECT_EQ("\tabc\n ", s);
  EXPECT_EQ("abc", r0);
  std::string r1 = TrimInPlace(&s);
  EXPECT_EQ("abc", s);
  EXPECT_EQ("abc", r1);
}

TEST(StrHelper, TrimLeftInPlace) {
  std::string s("\tabc\n ");
  std::string r = TrimLeftInPlace(&s);
  EXPECT_EQ("abc\n ", s);
  EXPECT_EQ("abc\n ", r);
}

TEST(StrHelper, TrimRightInPlace) {
  std::string s("\tabc\n ");
  std::string r = TrimRightInPlace(&s);
  EXPECT_EQ("\tabc", s);
  EXPECT_EQ("\tabc", r);
}

inline int EditDistanceTestHelper(const std::string &lhs,
                                  const std::string &rhs,
                                  int threshold) noexcept {
  return EditDistance(lhs.data(), lhs.size(),  //
                      rhs.data(), rhs.size(), threshold);
}

TEST(StrHelper, EditDistance) {
  EXPECT_EQ(EditDistanceTestHelper("", "a", 3), 1);
  EXPECT_EQ(EditDistanceTestHelper("a", "", 3), 1);
  EXPECT_EQ(EditDistanceTestHelper("a", "b", 3), 1);
  EXPECT_EQ(EditDistanceTestHelper("a", "abcdef", 3), -1);
  EXPECT_EQ(EditDistanceTestHelper("abcde", "efghi", 3), -1);  // jump quicker
  EXPECT_EQ(EditDistanceTestHelper("aaaabcdefg", "abaabcdefg", 3), 1);
  EXPECT_EQ(EditDistanceTestHelper("aaaabcdefg", "baabcdefg", 3), 2);
  EXPECT_EQ(EditDistanceTestHelper("aaaabcdefg", "baabcefg", 3), 3);
  EXPECT_EQ(EditDistanceTestHelper("aaaabcdefg", "baabcfg", 3), -1);  // 4
}

TEST(StrHelper, StrPrintf) {
  const std::string sdest = "abc12.345067";
  const std::string ssrc = StrPrintf("abc%2.2f5%02d%s", 12.34, 6, "7");
  EXPECT_EQ(sdest, ssrc);
}

}  // namespace argcv
