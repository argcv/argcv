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
#include "argcv/c/char/char_helper.h"

#include <string.h>

#include "gtest/gtest.h"

TEST(IsWesternSpace, PositiveCaseTest) {
  EXPECT_EQ(1, is_western_space(0x09));
  EXPECT_EQ(1, is_western_space(0x0A));
  EXPECT_EQ(1, is_western_space(0x0D));
  EXPECT_EQ(1, is_western_space(0x20));
  EXPECT_EQ(1, is_western_space(' '));
  EXPECT_EQ(1, is_western_space('\t'));
  EXPECT_EQ(1, is_western_space('\r'));
  EXPECT_EQ(1, is_western_space('\n'));
}

TEST(IsWesternSpace, NegativeCaseTest) {
  EXPECT_EQ(0, is_western_space('Z'));
  EXPECT_EQ(0, is_western_space('\0'));
}

TEST(IsWesternHyphen, CaseTest) {
  EXPECT_EQ(1, is_western_hyphen('-'));
  EXPECT_EQ(0, is_western_hyphen('+'));
}

TEST(IsWesternCharacter, CaseTest) {
  EXPECT_EQ(1, is_western_character('0'));
  EXPECT_EQ(1, is_western_character('9'));
  EXPECT_EQ(1, is_western_character('a'));
  EXPECT_EQ(1, is_western_character('z'));
  EXPECT_EQ(1, is_western_character('A'));
  EXPECT_EQ(1, is_western_character('Z'));
  EXPECT_EQ(0, is_western_character('+'));
}

TEST(Utf8CharsToUnicode, CaseTest) {
  unsigned char test[6];
  memset(test, 0, sizeof(unsigned char) * 6);
  EXPECT_EQ(0x00, utf8_chars_to_unicode(test, 1));
  test[0] = 0x7F;
  EXPECT_EQ(0x7F, utf8_chars_to_unicode(test, 1));
  test[0] = 0x81;
  test[1] = 0xBF;
  EXPECT_EQ(0x7F, utf8_chars_to_unicode(test, 2));
  test[0] = 0xFD;
  for (int i = 1; i < 6; i++) {
    test[i] = 0xBF;
  }
  EXPECT_EQ(0x7FFFFFFF, utf8_chars_to_unicode(test, 6));
}
