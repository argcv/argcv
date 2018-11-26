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
#include "argcv/cxx/str/str_slice.h"

#include <cstdio>
#include <iostream>
#include <map>

#include "gtest/gtest.h"

namespace argcv {

TEST(StrSlice, Append) {
  StrSlice sb;
  EXPECT_EQ(0, sb.size());
  StrSlice& op = sb.Append("1");
  EXPECT_EQ(1, sb.size());
  op.Append('2').Append("345");
  EXPECT_EQ(5, sb.size());
  EXPECT_EQ("12345", op.ToString());
  op.Append("6", '7', "8", "90");
  EXPECT_EQ("1234567890", op.ToString());
}

TEST(StrSlice, Append2) {
  StrSlice sb("12", std::string("3"), "4", '5');
  EXPECT_EQ("12345", sb.ToString());
  sb.Append("6", '7', "8", "90");
  EXPECT_EQ("1234567890", sb.ToString());
}

TEST(StrSlice, Copy) {
  StrSlice sb("12", std::string("3"), "4", '5');
  EXPECT_EQ("12345", sb.ToString());
  StrSlice sb2 = sb;
  EXPECT_EQ("12345", sb2.ToString());
  sb2.Reset();
  EXPECT_EQ("12345", sb.ToString());
}

TEST(StrSlice, Reverse) {
  StrSlice sb("12345");
  EXPECT_EQ("54321", sb.Reverse().ToString());
  EXPECT_EQ("12345", sb.Reverse().ToString());
  sb = "";
  EXPECT_EQ("", sb.Reverse().ToString());
  EXPECT_EQ("", sb.Reverse().ToString());
  sb = "1";
  EXPECT_EQ("1", sb.Reverse().ToString());
  EXPECT_EQ("1", sb.Reverse().ToString());
  sb = "12";
  EXPECT_EQ("21", sb.Reverse().ToString());
  EXPECT_EQ("12", sb.Reverse().ToString());
}

TEST(StrSlice, Reset) {
  StrSlice sb("1234");
  sb.Reset();
  EXPECT_EQ(0, sb.size());
  sb.Append("12", "345");
  EXPECT_EQ("12345", sb.ToString());
}

TEST(StrSlice, Compare) {
  StrSlice sbl("12345");
  StrSlice sbr("12345");
  EXPECT_TRUE(sbl == sbr);
  EXPECT_TRUE(sbl <= sbr);
  StrSlice sblp1("123456");
  StrSlice sblm1("1234");
  EXPECT_TRUE(sbl < sblp1);
  EXPECT_TRUE(sbl > sblm1);
  EXPECT_TRUE(sbl <= sblp1);
  EXPECT_TRUE(sbl >= sblm1);
  EXPECT_LT(sbl, sblp1);
  EXPECT_GT(sbl, sblm1);
  EXPECT_LE(sbl, sblp1);
  EXPECT_GE(sbl, sblm1);
  StrSlice sbrfcp("22345");
  StrSlice sbrfcm("02345");
  EXPECT_TRUE(sbl < sbrfcp);
  EXPECT_TRUE(sbl > sbrfcm);
  EXPECT_TRUE(sbl <= sbrfcp);
  EXPECT_TRUE(sbl >= sbrfcm);
  EXPECT_LT(sbl, sbrfcp);
  EXPECT_GT(sbl, sbrfcm);
  EXPECT_LE(sbl, sbrfcp);
  EXPECT_GE(sbl, sbrfcm);
}

TEST(StrSlice, Take) {
  StrSlice sb("1234567");
  sb.Take(5);
  EXPECT_EQ(sb, StrSlice("12345"));
  sb.Take(3);
  EXPECT_EQ(sb, StrSlice("123"));
  sb.Take(100);
  EXPECT_EQ(sb, StrSlice("123"));
}

TEST(StrSlice, Drop) {
  StrSlice sb("12345");
  sb.Drop(3);
  EXPECT_EQ(sb, StrSlice("45"));
  sb.Drop(10);
}

TEST(StrSlice, DropRight) {
  StrSlice sb("12345");
  sb.DropRight(3);
  EXPECT_EQ(sb, StrSlice("12"));
}

TEST(StrSlice, Slice) {
  StrSlice sb("12345");
  sb.Slice(1, 3);
  EXPECT_EQ(sb, StrSlice("234"));
}

TEST(StrSlice, Slice2) {
  StrSlice sb("12345");
  EXPECT_EQ(sb.Slice(1, 3), StrSlice("234"));
}

TEST(StrSlice, SubStr) {
  StrSlice sb("12345");
  EXPECT_EQ(sb.SubStr(1, 3), StrSlice("234"));
  EXPECT_EQ(sb, StrSlice("12345"));
}

TEST(StrSlice, StartsWith) {
  StrSlice sb("12345");
  EXPECT_TRUE(sb.StartsWith("1"));
  EXPECT_TRUE(sb.StartsWith("12"));
  EXPECT_TRUE(sb.StartsWith("123"));
  EXPECT_FALSE(sb.StartsWith("124"));
  StrSlice sb2;
  EXPECT_TRUE(sb.StartsWith(sb2.Append("1")));
  EXPECT_TRUE(sb.StartsWith(sb2.Append("2")));
  EXPECT_TRUE(sb.StartsWith(sb2.Append("3")));
  EXPECT_FALSE(sb.StartsWith(sb2.Append("5")));
}

TEST(StrSlice, EndsWith) {
  StrSlice sb("12345");
  EXPECT_TRUE(sb.EndsWith("5"));
  EXPECT_TRUE(sb.EndsWith("45"));
  EXPECT_TRUE(sb.EndsWith("345"));
  EXPECT_FALSE(sb.EndsWith("445"));
}

TEST(StrSlice, Contains) {
  StrSlice a("1234567");
  StrSlice b("1234");
  StrSlice c("567");
  StrSlice d("78");
  EXPECT_TRUE(a.Contains(b));
  EXPECT_TRUE(a.Contains(c));
  EXPECT_TRUE(!a.Contains(d));
  EXPECT_TRUE(a.Contains("1234"));
  EXPECT_TRUE(a.Contains("567"));
  EXPECT_TRUE(!a.Contains("78"));
}

TEST(StrSlice, Clone) {
  StrSlice sb("12345");
  auto sb2 = sb.Clone();
  sb.Append("abc");
  sb2.Append("def");
  EXPECT_EQ("12345abc", sb.ToString());
  EXPECT_EQ("12345def", sb2.ToString());
}

TEST(StrSlice, Write) {
  StrSlice a("1234567你好");
  std::cout << "[" << a << "]" << std::endl;
}

}  // namespace argcv
