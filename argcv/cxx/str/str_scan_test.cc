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
#include "argcv/cxx/str/str_scan.h"

#include <cstdio>

#include <map>
#include <string>

#include "argcv/cxx/str/str_slice.h"
#include "gtest/gtest.h"

namespace argcv {

class StrScanTest : public ::testing::Test {
 protected:
  // Returns a string with all chars that are in <clz>, in byte value order.
  std::string ClassStr(StrScan::CharClass clz) {
    std::string s;
    for (int i = 0; i < 256; ++i) {
      char ch = i;
      if (StrScan::Matches(clz, ch)) {
        s += ch;
      }
    }
    return s;
  }
};

TEST_F(StrScanTest, GetResult) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("   horse0123").GetResult(&remaining, &match));
  EXPECT_EQ("", match.ToString());
  EXPECT_EQ("   horse0123", remaining.ToString());
}

TEST_F(StrScanTest, Any) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("   horse0123")
                  .Any(StrScan::SPACE)
                  .Any(StrScan::DIGIT)
                  .Any(StrScan::LETTER)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("   horse", match.ToString());
  EXPECT_EQ("0123", remaining.ToString());

  EXPECT_TRUE(StrScan("")
                  .Any(StrScan::SPACE)
                  .Any(StrScan::DIGIT)
                  .Any(StrScan::LETTER)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.ToString());
  EXPECT_EQ("", match.ToString());

  EXPECT_TRUE(StrScan("----")
                  .Any(StrScan::SPACE)
                  .Any(StrScan::DIGIT)
                  .Any(StrScan::LETTER)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("----", remaining.ToString());
  EXPECT_EQ("", match.ToString());
}

TEST_F(StrScanTest, AnySpace) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("  a b ")
                  .AnySpace()
                  .One(StrScan::LETTER)
                  .AnySpace()
                  .GetResult(&remaining, &match));
  EXPECT_EQ("  a ", match.ToString());
  EXPECT_EQ("b ", remaining.ToString());
}

TEST_F(StrScanTest, AnyEscapedNewline) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("\\\n")
                  .Any(StrScan::LETTER_DIGIT_UNDERSCORE)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("\\\n", remaining.ToString());
  EXPECT_EQ("", match.ToString());
}

TEST_F(StrScanTest, AnyEmptyString) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("")
                  .Any(StrScan::LETTER_DIGIT_UNDERSCORE)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.ToString());
  EXPECT_EQ("", match.ToString());
}

TEST_F(StrScanTest, Eos) {
  EXPECT_FALSE(StrScan("a").Eos().GetResult());
  EXPECT_TRUE(StrScan("").Eos().GetResult());
  EXPECT_FALSE(StrScan("abc").OneLiteral("ab").Eos().GetResult());
  EXPECT_TRUE(StrScan("abc").OneLiteral("abc").Eos().GetResult());
}

TEST_F(StrScanTest, Many) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("abc").Many(StrScan::LETTER).GetResult());
  EXPECT_FALSE(StrScan("0").Many(StrScan::LETTER).GetResult());
  EXPECT_FALSE(StrScan("").Many(StrScan::LETTER).GetResult());

  EXPECT_TRUE(
      StrScan("abc ").Many(StrScan::LETTER).GetResult(&remaining, &match));
  EXPECT_EQ(" ", remaining.underlying_ref());
  EXPECT_EQ("abc", match.underlying_ref());
  EXPECT_TRUE(
      StrScan("abc").Many(StrScan::LETTER).GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.underlying_ref());
  EXPECT_EQ("abc", match.underlying_ref());
}

TEST_F(StrScanTest, One) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan("abc").One(StrScan::LETTER).GetResult());
  EXPECT_FALSE(StrScan("0").One(StrScan::LETTER).GetResult());
  EXPECT_FALSE(StrScan("").One(StrScan::LETTER).GetResult());

  EXPECT_TRUE(StrScan("abc")
                  .One(StrScan::LETTER)
                  .One(StrScan::LETTER)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("c", remaining.underlying_ref());
  EXPECT_EQ("ab", match.underlying_ref());
  EXPECT_TRUE(StrScan("a").One(StrScan::LETTER).GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.underlying_ref());
  EXPECT_EQ("a", match.underlying_ref());
}

TEST_F(StrScanTest, OneLiteral) {
  EXPECT_FALSE(StrScan("abc").OneLiteral("abC").GetResult());
  EXPECT_TRUE(StrScan("abc").OneLiteral("ab").OneLiteral("c").GetResult());
}

TEST_F(StrScanTest, ScanUntil) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan(R"(' \1 \2 \3 \' \\'rest)")
                  .OneLiteral("'")
                  .ScanUntil('\'')
                  .OneLiteral("'")
                  .GetResult(&remaining, &match));
  EXPECT_EQ(R"( \\'rest)", remaining.ToString());
  EXPECT_EQ(R"(' \1 \2 \3 \')", match.ToString());

  // The "scan until" character is not present.
  remaining.Reset().Append("unset");
  match.Reset().Append("unset");
  EXPECT_FALSE(StrScan(R"(' \1 \2 \3 \\rest)")
                   .OneLiteral("'")
                   .ScanUntil('\'')
                   .GetResult(&remaining, &match));
  EXPECT_EQ("unset", remaining.ToString());
  EXPECT_EQ("unset", match.ToString());

  // Scan until an escape character.
  remaining.Reset();
  match.Reset();
  EXPECT_TRUE(
      StrScan(R"(123\456)").ScanUntil('\\').GetResult(&remaining, &match));
  EXPECT_EQ(R"(\456)", remaining.ToString());
  EXPECT_EQ("123", match.ToString());
}

TEST_F(StrScanTest, ScanEscapedUntil) {
  StrSlice remaining, match;
  EXPECT_TRUE(StrScan(R"(' \1 \2 \3 \' \\'rest)")
                  .OneLiteral("'")
                  .ScanEscapedUntil('\'')
                  .OneLiteral("'")
                  .GetResult(&remaining, &match));
  EXPECT_EQ("rest", remaining.ToString());
  EXPECT_EQ(R"(' \1 \2 \3 \' \\')", match.ToString());

  // The "scan until" character is not present.
  remaining.Reset().Append("unset");
  match.Reset().Append("unset");
  EXPECT_FALSE(StrScan(R"(' \1 \2 \3 \' \\rest)")
                   .OneLiteral("'")
                   .ScanEscapedUntil('\'')
                   .GetResult(&remaining, &match));
  EXPECT_EQ("unset", remaining.ToString());
  EXPECT_EQ("unset", match.ToString());
}

TEST_F(StrScanTest, ZeroOrOneLiteral) {
  StrSlice remaining, match;
  EXPECT_TRUE(
      StrScan("abc").ZeroOrOneLiteral("abC").GetResult(&remaining, &match));
  EXPECT_EQ("abc", remaining.ToString());
  EXPECT_EQ("", match.ToString());

  EXPECT_TRUE(
      StrScan("abcd").ZeroOrOneLiteral("ab").ZeroOrOneLiteral("c").GetResult(
          &remaining, &match));
  EXPECT_EQ("d", remaining.ToString());
  EXPECT_EQ("abc", match.ToString());

  EXPECT_TRUE(
      StrScan("").ZeroOrOneLiteral("abc").GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.ToString());
  EXPECT_EQ("", match.ToString());
}

// Test output of GetResult (including the forms with optional params),
// and that it can be called multiple times.
TEST_F(StrScanTest, CaptureAndGetResult) {
  StrSlice remaining, match;

  StrScan scan("  first    second");
  EXPECT_TRUE(scan.Any(StrScan::SPACE)
                  .RestartCapture()
                  .One(StrScan::LETTER)
                  .Any(StrScan::LETTER_DIGIT)
                  .StopCapture()
                  .Any(StrScan::SPACE)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("second", remaining.ToString());
  EXPECT_EQ("first", match.ToString());
  EXPECT_TRUE(scan.GetResult());
  remaining = "";
  EXPECT_TRUE(scan.GetResult(&remaining));
  EXPECT_EQ("second", remaining.ToString());
  remaining = "";
  match = "";
  EXPECT_TRUE(scan.GetResult(&remaining, &match));
  EXPECT_EQ("second", remaining.ToString());
  EXPECT_EQ("first", match.ToString());

  scan.RestartCapture().One(StrScan::LETTER).One(StrScan::LETTER);
  remaining = "";
  match = "";
  EXPECT_TRUE(scan.GetResult(&remaining, &match));
  EXPECT_EQ("cond", remaining.ToString());
  EXPECT_EQ("se", match.ToString());
}

// Tests that if StopCapture is not called, then calling GetResult, then
// scanning more, then GetResult again will update the capture.
TEST_F(StrScanTest, MultipleGetResultExtendsCapture) {
  StrSlice remaining, match;

  StrScan scan("one2three");
  EXPECT_TRUE(scan.Many(StrScan::LETTER).GetResult(&remaining, &match));
  EXPECT_EQ("2three", remaining.ToString());
  EXPECT_EQ("one", match.ToString());
  EXPECT_TRUE(scan.Many(StrScan::DIGIT).GetResult(&remaining, &match));
  EXPECT_EQ("three", remaining.ToString());
  EXPECT_EQ("one2", match.ToString());
  EXPECT_TRUE(scan.Many(StrScan::LETTER).GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.ToString());
  EXPECT_EQ("one2three", match.ToString());
}

TEST_F(StrScanTest, FailedMatchDoesntChangeResult) {
  // A failed match doesn't change pointers passed to GetResult.
  StrScan scan("name");
  StrSlice remaining("rem");
  StrSlice match("match");
  EXPECT_FALSE(scan.One(StrScan::SPACE).GetResult(&remaining, &match));
  EXPECT_EQ("rem", remaining.ToString());
  EXPECT_EQ("match", match.ToString());
}

TEST_F(StrScanTest, DefaultCapturesAll) {
  // If RestartCapture() is not called, the whole string is used.
  StrScan scan("a b");
  StrSlice remaining("rem");
  StrSlice match("match");
  EXPECT_TRUE(scan.Any(StrScan::LETTER)
                  .AnySpace()
                  .Any(StrScan::LETTER)
                  .GetResult(&remaining, &match));
  EXPECT_EQ("", remaining.ToString());
  EXPECT_EQ("a b", match.ToString());
}

TEST_F(StrScanTest, AllCharClasses) {
  EXPECT_EQ(256, ClassStr(StrScan::ALL).size());
  EXPECT_EQ("0123456789", ClassStr(StrScan::DIGIT));
  EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER));
  EXPECT_EQ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER_DIGIT));
  EXPECT_EQ(
      "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
      "abcdefghijklmnopqrstuvwxyz",
      ClassStr(StrScan::LETTER_DIGIT_DASH_UNDERSCORE));
  EXPECT_EQ(
      "-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz",
      ClassStr(StrScan::LETTER_DIGIT_DASH_DOT_SLASH));
  EXPECT_EQ(
      "-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
      "abcdefghijklmnopqrstuvwxyz",
      ClassStr(StrScan::LETTER_DIGIT_DASH_DOT_SLASH_UNDERSCORE));
  EXPECT_EQ(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER_DIGIT_DOT));
  EXPECT_EQ("+-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER_DIGIT_DOT_PLUS_MINUS));
  EXPECT_EQ(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER_DIGIT_DOT_UNDERSCORE));
  EXPECT_EQ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LETTER_DIGIT_UNDERSCORE));
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", ClassStr(StrScan::LOWERLETTER));
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LOWERLETTER_DIGIT));
  EXPECT_EQ("0123456789_abcdefghijklmnopqrstuvwxyz",
            ClassStr(StrScan::LOWERLETTER_DIGIT_UNDERSCORE));
  EXPECT_EQ("123456789", ClassStr(StrScan::NON_ZERO_DIGIT));
  EXPECT_EQ("\t\n\v\f\r ", ClassStr(StrScan::SPACE));
  EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", ClassStr(StrScan::UPPERLETTER));
}

TEST_F(StrScanTest, Peek) {
  EXPECT_EQ('a', StrScan("abc").Peek());
  EXPECT_EQ('a', StrScan("abc").Peek('b'));
  EXPECT_EQ('\0', StrScan("").Peek());
  EXPECT_EQ('z', StrScan("").Peek('z'));
  EXPECT_EQ('A', StrScan("0123A").Any(StrScan::DIGIT).Peek());
  EXPECT_EQ('\0', StrScan("0123A").Any(StrScan::LETTER_DIGIT).Peek());
}

}  // namespace argcv
