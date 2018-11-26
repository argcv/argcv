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
#ifndef ARGCV_CXX_STR_STR_HELPER_H_
#define ARGCV_CXX_STR_STR_HELPER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace argcv {

std::vector<std::string> Split(const std::string &s, const std::string &delim,
                               bool skip_blank_elem = true) noexcept;

static inline std::string &ReplaceAllInPlace(
    std::string *str_raw, const std::string &str_src,
    const std::string &str_dst) noexcept {
  std::string::size_type pos = 0;
  std::string::size_type src_len = str_src.size();
  std::string::size_type dst_len = str_dst.size();
  while ((pos = str_raw->find(str_src, pos)) != std::string::npos) {
    str_raw->replace(pos, src_len, str_dst);
    pos += dst_len;
  }
  return *str_raw;
}

static inline std::string ReplaceAll(std::string str_raw,
                                     const std::string &str_src,
                                     const std::string &str_dst) noexcept {
  return ReplaceAllInPlace(&str_raw, str_src, str_dst);
}

// Given a string, cut it into a few chunks,
// Each element in vector is a utf-8 character
std::vector<std::string> Utf8Split(const std::string &s) noexcept;

// Given a string, cut it into a unicode vector
// Each element in vector is a unicode in uint64_t
std::vector<uint64_t> Utf8ToUnicode(const std::string &s) noexcept;

// BasicStrSlice<T> &Drop(size_t n) noexcept {
//   data_.erase(0, n);
//   return *this;
// }

static inline std::string &TrimLeftIf(
    std::string *str_raw, std::function<bool(char)> to_remove) noexcept {
  std::string::size_type i;
  size_t upper = str_raw->size();
  for (i = 0; i < upper && to_remove((*str_raw)[i]); i++) {
  }
  if (i > 0) str_raw->erase(0, i);
  return *str_raw;
}

static inline std::string &TrimRightIf(
    std::string *str_raw, std::function<bool(char)> to_remove) noexcept {
  std::string::size_type i;
  for (i = str_raw->size(); i > 0 && to_remove((*str_raw)[i - 1]); i--) {
  }
  str_raw->resize(i);
  return *str_raw;
}

static inline std::string &TrimLeftInPlace(std::string *str_raw) noexcept {
  return TrimLeftIf(
      str_raw, [](char ch) -> bool { return isspace(static_cast<int>(ch)); });
}

static inline std::string &TrimRightInPlace(std::string *str_raw) noexcept {
  return TrimRightIf(
      str_raw, [](char ch) -> bool { return isspace(static_cast<int>(ch)); });
}

static inline std::string &TrimInPlace(std::string *str_raw) noexcept {
  TrimLeftInPlace(str_raw);
  return TrimRightInPlace(str_raw);
}

static inline std::string &Trim(std::string str_raw) noexcept {
  return TrimInPlace(&str_raw);
}

/** \brief Calculate the edit distance between 2 strings
 * \param lhs string 1
 * \param len_lhs an integer setting the size of string 1
 * \param rhs string 2
 * \param len_rhs an integer setting the size of string 2
 * \param threshold return -1 in case of the ed is > this value
 * \return edit distance or -1
 *
 * Calculate the edit distance between 2 strings
 * return -1 or a positive number no more than threshold
 */
static inline int EditDistance(const char *lhs, int len_lhs, const char *rhs,
                               int len_rhs, int threshold) noexcept {
  if (len_lhs - len_rhs > threshold || len_rhs - len_lhs > threshold) return -1;
  int dist[len_lhs + 1][len_rhs + 1];
  memset(dist, 0, sizeof(int) * (len_lhs + 1) * (len_rhs + 1));
  for (int i = 0; i <= len_lhs; i++) {
    dist[i][0] = i;
  }
  for (int i = 0; i <= len_rhs; i++) dist[0][i] = i;

  for (int i = 1; i <= len_lhs; i++) {
    int st = std::max(1, i - threshold);
    int en = std::min(len_rhs, i + threshold);
    bool flag = true;
    for (int j = st; j <= en; j++) {
      dist[i][j] = threshold + 1;
      if (j - i + 1 <= threshold && dist[i - 1][j] + 1 < dist[i][j])
        dist[i][j] = dist[i - 1][j] + 1;
      if (i - j + 1 <= threshold && dist[i][j - 1] + 1 < dist[i][j])
        dist[i][j] = dist[i][j - 1] + 1;
      dist[i][j] = std::min(
          dist[i][j], dist[i - 1][j - 1] + (lhs[i - 1] != rhs[j - 1] ? 1 : 0));
      if (dist[i][j] <= threshold) flag = false;
    }
    if (flag && i > threshold) {
      return -1;
    }
  }
  return dist[len_lhs][len_rhs] > threshold ? -1 : dist[len_lhs][len_rhs];
}

// /**
//  * load a whole file to string
//  *
//  */
// inline std::string LoadFile(const std::string &name) {
//   FILE *fp = fopen(name.c_str(), "rb");
//   size_t sz;
//   char *buff;
//   fseek(fp, 0, SEEK_END);
//   sz = ftell(fp);
//   fseek(fp, 0, SEEK_SET);
//   // buff = (char *)malloc(sizeof(char) * (sz + 1));
//   buff = reinterpret_cast<char *>(malloc(sizeof(char) * (sz + 1)));
//   buff[sz] = '\0';
//   fread(buff, sz, 1, fp);
//   std::string rtstr(buff, sz);
//   free(buff);
//   fclose(fp);
//   return rtstr;
// }

template <typename... Args>
std::string StrPrintf(const std::string &format, Args... args) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
  // https://goo.gl/WCNP6U
  size_t size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
#pragma clang diagnostic pop
  std::unique_ptr<char[]> buf(new char[size]);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
  std::snprintf(buf.get(), size, format.c_str(), args...);
#pragma clang diagnostic pop
  return std::string(buf.get(), buf.get() + size - 1);
}

}  // namespace argcv

#endif  // ARGCV_CXX_STR_STR_HELPER_H_
