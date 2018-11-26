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

#include <stddef.h>
#include <stdint.h>

int is_western_space(char ch) {
  switch (ch) {
    case 0x09:
    case 0x0A:
    case 0x0D:
    case 0x20:
      return 1;
    default:
      return 0;
  }
}

int is_western_punct(char ch) {
  // see if it is either a space, control character,
  // or alphanumeric and negate that
  return ch > 0 && !(((ch >= 0x30 /*'0'*/) && (ch <= 0x39 /*'9'*/)) ||
                     ((ch >= 0x41 /*'A'*/) && (ch <= 0x5A /*'Z'*/)) ||
                     ((ch >= 0x61 /*'a'*/) && (ch <= 0x7A /*'z'*/)) ||
                     ((ch >= 0x00) && (ch <= 0x20)));
}

int is_western_hyphen(char ch) { return ch == 0x2D; }

int is_western_character(char ch) {
  return (((ch >= 0x30 /*'0'*/) && (ch <= 0x39 /*'9'*/)) ||
          ((ch >= 0x41 /*'A'*/) && (ch <= 0x5A /*'Z'*/)) ||
          ((ch >= 0x61 /*'a'*/) && (ch <= 0x7A /*'z'*/)));
}

int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

int utf8_char_length(unsigned char ch) {
  unsigned char t = 0x80;
  if (!(ch & t))  // ! 1000 0000 B
    return 1;
  t = t >> 1;
  if (!(ch & t))  // ! 0100 0000 B
    return 0;
  t = t >> 1;
  int l = 2;
  while (ch & t) {
    t = t >> 1;
    l++;
    if (l > 6) return 0;
  }
  return l;
}

uint64_t utf8_chars_to_unicode(const unsigned char *in, size_t n) {
  if (n == 0 || n > 6 || in == NULL) {
    return 0L;
  }
  uint64_t ret = (uint64_t)in[0];
  // if in[0] in [0x00000000,0x01111111]
  // it is a single code, return directly
  if (ret < 0x80) {
    return ret;
  }

  // for:
  // 110xxxxx 10xxxxxx
  // 1110xxxx 10xxxxxx 10xxxxxx
  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
  // ...
  // 1111110x 10xxxxxx * 5
  uint64_t mask = (1 << (8 - n)) - 1;
  ret &= mask;

  // C0: 11000000
  size_t ix = 1;
  while (ix < n) {
    unsigned char ch = in[ix++];
    // double check ch
    // ch must be 10xxx xxxx
    if ((ch & 0x80) != 0x80) {
      return 0L;
    }
    ret <<= 6;
    ret |= ch & 0x3F;
  }

  return ret;
}
