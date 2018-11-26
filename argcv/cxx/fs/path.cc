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
#include "argcv/cxx/fs/path.h"

#include <cstring>  // memcpy(void *restrict dst, const void *restrict src, size_t n);
#include <string>

namespace argcv {

PathBuff::PathBuff(const std::string &data, size_t len) noexcept
    : sz_(len), sz_buff_(2 * len + 1) {
  buff_ = new char[sz_buff_];
  if (sz_ > 0) {
    memcpy(buff_, data.data(), sz_);
  }
}

PathBuff &PathBuff::Append(char c) noexcept {
  if (sz_ >= sz_buff_) {
    sz_buff_ = sz_buff_ * 2 + 1;
    char *new_buff = new char[sz_buff_];
    memcpy(new_buff, buff_, sz_);
    delete[] buff_;
    buff_ = new_buff;
  }
  buff_[sz_] = c;
  sz_++;
  return *this;
}

}  // namespace argcv
