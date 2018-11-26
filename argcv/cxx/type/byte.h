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
#ifndef ARGCV_CXX_TYPE_BYTE_H_
#define ARGCV_CXX_TYPE_BYTE_H_

#include <cassert>
#include <cstdint>
// #include <cstdio>   // print
#include <cstdlib>  // memcmp
#include <cstring>  // memset
#include <iostream>
// #include <memory> // shared_ptr?
#include <string>
#include <vector>

#include "argcv/cxx/env/macros.h"

namespace argcv {

typedef uint8_t Byte;

class Bytes {
 public:
  Bytes() noexcept : data_(nullptr), size_(0) {}

  explicit Bytes(const std::string &data) noexcept
      : Bytes(reinterpret_cast<const Byte *>(data.data()), data.size()) {}

  explicit Bytes(const Byte *bytes, size_t size) noexcept : Bytes() {
    this->assign(bytes, size);
  }

  Bytes(const Bytes &bytes) noexcept { this->assign(bytes.data_, bytes.size_); }

  virtual ~Bytes() {
    // fprintf(stderr, "EXIT: %s:%d ... Bytes\n", __FILE__, __LINE__);
    clear();
  }

  const size_t size() const noexcept { return size_; }

  const Byte *data() const noexcept { return data_; }

  Byte *underlying_ref() const noexcept { return data_; }

  // return is empty
  const bool Empty() const noexcept { return data_ == nullptr || size() == 0; }

  std::string ToString() const noexcept {
    return data_ == nullptr
               ? std::string()
               : std::string(reinterpret_cast<const char *>(data()), size());
  }

  std::string ToHexString() const noexcept {
    size_t n = size_;
    char *cout = new char[n * 2];
    const char *rune = "0123456789abcdef";
    for (size_t i = 0; i < n; i++) {
      Byte uc = data_[i];
      // little-endian
      cout[2 * i] = rune[uc >> 4];
      cout[2 * i + 1] = rune[uc & 0xf];
    }
    std::string out(cout, n * 2);
    delete[] cout;
    return out;
  }

  int Compare(const Bytes &that) const noexcept {
    size_t sz = size();
    size_t that_sz = that.size();
    size_t min_sz = (sz < that_sz) ? sz : that_sz;
    int r = memcmp(data(), that.data(), min_sz);
    return r != 0 ? r : (sz == that_sz ? 0 : (sz < that_sz ? -1 : +1));
  }

  OP_COMPARABLE(Bytes);

  // Return the ith byte in the referenced data.
  // REQUIRES: n < size()
  Byte operator[](size_t n) const noexcept {
    assert(n < size());
    return data_[n];
  }

  // Given a byte array, decode from a hex string
  // aka: Parse a hex string
  bool FromHexString(const std::string &in) noexcept;

  template <typename T>
  bool Write(T *t, size_t offset) const noexcept {
    if (t == nullptr) {
      return false;
    }
    size_t n = sizeof(T);
    size_t to = n + offset;
    if (to > size()) {
      return false;
    }
    write<T>(t, offset, to, n);
    return true;
  }

  template <typename T>
  bool Write(std::vector<T> *v) const noexcept {
    if (v == nullptr) {
      return false;
    }
    size_t n = sizeof(T);
    size_t str_len = size();
    if (str_len == 0 || str_len % n != 0) {
      return false;
    }
    for (size_t i = 0; i < str_len; i += n) {
      T t;
      size_t to = i + n;
      write<T>(&t, i, to, n);
      v->push_back(t);
    }
    return true;
  }

  Byte *Resize(size_t size) noexcept {
    Byte *new_data = new Byte[size];
    memset(new_data, 0, size);
    if (this->data_ != nullptr) {
      size_t min_sz = (size < this->size_) ? size : this->size_;
      memcpy(static_cast<void *>(new_data),
             static_cast<const void *>(this->data_), min_sz);
      delete[] this->data_;
    }
    this->data_ = new_data;
    this->size_ = size;
    return data_;
  }

 private:
  // Cons:
  // Is 'shared_ptr' a better container?
  // http://en.cppreference.com/w/cpp/memory/default_delete
  // std::shared_ptr<char> data_(new uint8_t[size_],
  // std::default_delete<uint8_t[]>());
  Byte *data_;
  size_t size_;

  template <typename T>
  void write(T *t, size_t offset, size_t to, size_t n) const noexcept {
    memset(t, 0, n);
    for (size_t i = offset; i < to; i++) {
      *t = (*t) << 8 | data_[i];
    }
  }

  void clear() noexcept {
    if (data_ != nullptr) {
      delete[] data_;
    }
    data_ = nullptr;
    size_ = 0;
  }

  void assign(const Byte *bytes, size_t size) noexcept {
    clear();
    size_ = size;
    data_ = new Byte[size];
    memcpy(static_cast<void *>(data_), static_cast<const void *>(bytes), size);
  }
};

static inline std::ostream &operator<<(std::ostream &o, const Bytes &barr) {
  std::string data = barr.ToHexString();
  o.write(data.data(), data.size());
  return o;
}

// Encode One Type to little-endian string
template <typename T>
Bytes ToBytes(T v) noexcept {
  size_t n = sizeof(T);
  // Byte *data = new Byte[n];
  // memset(data, 0, n);
  Bytes bytes;
  bytes.Resize(n);
  Byte *data = bytes.underlying_ref();  // new Byte[data_len];
  for (size_t i = 1; i <= n; i++) {
    data[n - i] = v & 0xff;
    v >>= 8;
  }
  // Bytes bytes(data, n);
  // delete[] data;
  return bytes;
}

// Encode A vector to little-endian string
template <typename T>
Bytes ToBytes(const std::vector<T> &v) {
  size_t arr_len = v.size();
  size_t n = sizeof(T);
  size_t data_len = arr_len * n;
  Bytes bytes;
  bytes.Resize(data_len);
  Byte *data = bytes.underlying_ref();  // new Byte[data_len];
  // memset(data, 0, data_len);
  for (size_t j = 0; j < arr_len; j++) {
    T cv = v[j];
    size_t base_off = (j + 1) * n;
    for (size_t i = 1; i <= n; i++) {
      data[base_off - i] = cv & 0xff;
      cv >>= 8;
    }
  }
  // Bytes bytes(data, data_len);
  // delete[] data;
  return bytes;
}

// Encode A initializer_list to little-endian string
template <typename T>
Bytes ToBytes(std::initializer_list<T> il) {
  std::vector<T> v;
  for (auto i : il) {
    v.push_back(i);
  }
  return ToBytes<T>(v);
}

}  // namespace argcv

#endif  //  ARGCV_CXX_TYPE_BYTE_H_
