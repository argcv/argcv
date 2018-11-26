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
#ifndef ARGCV_CXX_STR_STR_SLICE_H_
#define ARGCV_CXX_STR_STR_SLICE_H_

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "argcv/cxx/env/macros.h"

namespace argcv {

class StrSlice {
 public:
  typedef size_t size_type;
  static const size_t npos = size_type(-1);

  StrSlice() noexcept {}
  explicit StrSlice(const std::string &data) noexcept : data_(data) {}

  template <class... Args>
  explicit StrSlice(const Args &... args) noexcept {
    this->Append(args...);
  }

  StrSlice(const StrSlice &data) noexcept : data_(data.data_) {}
  StrSlice(const StrSlice &&data) : data_(std::move(data.data_)) {}

  StrSlice &Append(const std::string &r) noexcept {
    data_ += r;
    return *this;
  }

  StrSlice &Append(const char *r) noexcept {
    // printf("Append! %s:%d\n", __FILE__, __LINE__);
    data_ += r;
    return *this;
  }

  StrSlice &Append(char r) noexcept {
    // printf("Append! %s:%d\n", __FILE__, __LINE__);
    data_ += r;
    return *this;
  }

  StrSlice &Append(std::initializer_list<char> r) noexcept {
    // printf("Append! %s:%d\n", __FILE__, __LINE__);
    data_ += r;
    return *this;
  }

  StrSlice &Append(const StrSlice &r) noexcept {
    // printf("Append! %s:%d\n", __FILE__, __LINE__);
    data_ += r.data_;
    return *this;
  }

  StrSlice &Append(std::initializer_list<const std::string> rest) noexcept {
    // printf("Append! %s:%d\n", __FILE__, __LINE__);
    for (auto r : rest) {
      data_ += r;
    }
    return *this;
  }

  StrSlice &Append(const void *data, size_t len) noexcept {
    std::string tmp(reinterpret_cast<const char *>(data), len);
    data_ += tmp;
    return *this;
  }

  template <class First, class... Rest>
  StrSlice &Append(const First &first, const Rest &... rest) noexcept {
    this->Append(first);
    this->Append(rest...);
    return *this;
  }

  const char *data() const noexcept { return data_.data(); }

  const size_t size() const noexcept { return data_.size(); }

  // return is empty
  const bool Empty() const noexcept { return size() == 0; }

  std::string &underlying_ref() noexcept { return data_; }

  std::string ToString() const noexcept { return data_; }

  StrSlice &Drop(size_t n) noexcept {
    data_.erase(0, n);
    return *this;
  }

  StrSlice &DropRight(size_t n) noexcept {
    data_.erase(data_.end() - n, data_.end());
    return *this;
  }

  StrSlice &Take(size_t n) noexcept {
    if (n < data_.size()) {
      data_.erase(data_.begin() + n, data_.end());
      return *this;
    } else {
      return *this;
    }
  }

  StrSlice &Slice(size_t offset, size_t size) noexcept {
    return (*this).Drop(offset).Take(size);
  }

  StrSlice &Reverse() noexcept {
    auto begin = data_.begin();
    auto end = data_.end() - 1;
    while (begin < end) {
      std::iter_swap(begin, end);
      begin++;
      end--;
    }
    return *this;
  }

  StrSlice SubStr(size_t offset, size_t size) const noexcept {
    return StrSlice(data_.substr(offset, size));
  }

  char At(size_t offset) { return data_.at(offset); }

  StrSlice &Reset() noexcept {
    std::string empty;
    data_.swap(empty);
    return *this;
  }

  StrSlice Clone() const noexcept {
    StrSlice sc2(*this);
    return sc2;
  }

  // Return true if "x" is a prefix of "*this"
  bool StartsWith(const StrSlice &x) const noexcept {
    return ((size() >= x.size()) && (memcmp(data(), x.data(), x.size()) == 0));
  }

  // Return true if "x" is a suffix of "*this"
  bool EndsWith(const StrSlice &x) const noexcept {
    return ((size() >= x.size()) &&
            (memcmp(data() + (size() - x.size()), x.data(), x.size()) == 0));
  }

  bool Contains(const StrSlice &x) const noexcept {
    return std::search(data_.begin(), data_.end(), x.data_.begin(),
                       x.data_.end()) != data_.end();
  }

  // Return true if "x" is a prefix of "*this"
  bool StartsWith(const std::string &x) const noexcept {
    return ((size() >= x.size()) && (memcmp(data(), x.data(), x.size()) == 0));
  }

  // Return true if "x" is a suffix of "*this"
  bool EndsWith(const std::string &x) const noexcept {
    return ((size() >= x.size()) &&
            (memcmp(data() + (size() - x.size()), x.data(), x.size()) == 0));
  }

  bool Contains(const std::string &x) const noexcept {
    return std::search(data_.begin(), data_.end(), x.begin(), x.end()) !=
           data_.end();
  }

  inline int Compare(const StrSlice &that) const noexcept {
    size_t sz = size();
    size_t that_sz = that.size();
    const size_t min_sz = (sz < that_sz) ? sz : that_sz;
    int r = memcmp(data(), that.data(), min_sz);
    return r != 0 ? r : (sz == that_sz ? 0 : (sz < that_sz ? -1 : +1));
  }

  inline int Compare(const std::string &that) const noexcept {
    size_t sz = size();
    size_t that_sz = that.size();
    const size_t min_sz = (sz < that_sz) ? sz : that_sz;
    int r = memcmp(data(), that.data(), min_sz);
    return r != 0 ? r : (sz == that_sz ? 0 : (sz < that_sz ? -1 : +1));
  }

  // Return the ith byte in the referenced data.
  // REQUIRES: n < size()
  char operator[](size_t n) const noexcept {
    assert(n < size());
    return data_[n];
  }

  StrSlice &operator=(const std::string &data) noexcept {
    data_ = data;
    return *this;
  }

  StrSlice &operator=(const StrSlice &data) noexcept {
    data_ = data.data_;
    return *this;
  }

  OP_COMPARABLE(StrSlice);
  OP_COMPARABLE(std::string);

 private:
  std::string data_;
};

static inline std::ostream &operator<<(std::ostream &o,
                                       const StrSlice &slice) noexcept {
  o.write(slice.data(), slice.size());
  return o;
}

}  // namespace argcv

#endif  // ARGCV_CXX_STR_STR_SLICE_H_
