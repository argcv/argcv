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
#include <string>
#include <vector>

#include "argcv/c/char/char_helper.h"

namespace argcv {

inline std::vector<std::string> &split(const std::string &s,
                                       const std::string &delim,
                                       std::vector<std::string> *_elems,
                                       bool skip_blank_elem = true) noexcept {
  std::string item;
  if (delim.empty()) {
    _elems->push_back(s);
    return *_elems;
  }
  std::string::const_iterator substart = s.begin(), subend;
  while (true) {
    subend = search(substart, s.end(), delim.begin(), delim.end());
    std::string temp(substart, subend);
    if (!skip_blank_elem || !temp.empty()) {
      _elems->push_back(temp);
    }
    if (subend == s.end()) {
      break;
    }
    substart = subend + delim.size();
  }
  return *_elems;
}

std::vector<std::string> Split(const std::string &s, const std::string &delim,
                               bool skip_blank_elem) noexcept {
  std::vector<std::string> elems;
  split(s, delim, &elems, skip_blank_elem);
  return elems;
}

inline bool byte2hex(int8_t *c, char in) noexcept {
  // how about use a map instead?
  if ('0' <= in && in <= '9') {
    *c = in - '0';
    return true;
  }

  if ('a' <= in && in <= 'z') {
    *c = in - 'a' + 10;
    return true;
  }

  if ('A' <= in && in <= 'Z') {
    *c = in - 'A' + 10;
    return true;
  }

  return false;
}

bool DecodeHexString(std::string *out, const std::string &in) noexcept {
  if (out == nullptr) {
    return false;
  }
  size_t n = in.size();
  if (n % 2 == 1) {
    // invalid length
    return false;
  }
  // n /= 2;
  n >>= 1;
  char *cout = new char[n];
  for (size_t i = 0; i < n; i++) {
    // little-endian
    int8_t hi, lo;
    if (byte2hex(&hi, in[2 * i]) && byte2hex(&lo, in[2 * i + 1])) {
      cout[i] = static_cast<char>(((hi << 4) & 0xf0) | (lo & 0x0f));
    }
  }
  std::string iout(reinterpret_cast<char *>(cout), n);
  delete[] cout;
  out->swap(iout);
  return true;
}

std::vector<std::string> Utf8Split(const std::string &s) noexcept {
  std::vector<std::string> elems;
  size_t clen = 0;
  size_t slen = s.size();
  for (size_t i = 0; i < slen; i++) {
    if ((clen = utf8_char_length(s[i]))) {
      elems.push_back(s.substr(i, clen));
      i += clen - 1;
    } else {
      // fprintf(stderr, "error size: %zu %zu\n", clen, i);
      // break;
      continue;
    }
  }
  return elems;
}

std::vector<uint64_t> Utf8ToUnicode(const std::string &s) noexcept {
  std::vector<uint64_t> elems;
  size_t n = s.length();
  for (size_t i = 0; i < n; i++) {
    size_t clen = utf8_char_length(s[i]);
    if (clen) {
      unsigned char in[clen];
      for (int j = i, k = 0; j < (i + clen);) {
        in[k++] = s[j++];
      }
      uint64_t ucode = utf8_chars_to_unicode(in, clen);
      if (ucode) {
        elems.push_back(ucode);
      } else {
        // errored here..
        continue;
      }
      i += clen - 1;
    } else {
      // fprintf(stderr, "error size: %zu %zu\n", clen, i);
      // errored
      // break;
      continue;
    }
  }
  return elems;
}

}  // namespace argcv
