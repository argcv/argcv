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
#ifndef ARGCV_CXX_CRYPTO_MD4_H_
#define ARGCV_CXX_CRYPTO_MD4_H_

#include <openssl/md4.h>

#include <cstdio>
#include <string>

#include "argcv/cxx/crypto/base.h"
#include "argcv/cxx/type/byte.h"

namespace argcv {

class MD4Helper : public BaseHasher {
 public:
  MD4Helper() noexcept {
    ptr_.Resize(MD4_DIGEST_LENGTH);
    MD4_Init(&ctx_);
  }

  virtual ~MD4Helper() {
    // delete[] md_;
  }

  bool Update(const std::string &str) noexcept {
    return MD4_Update(&ctx_, static_cast<const void *>(str.data()),
                      str.length()) == 1;
  }

  // bool Finish() noexcept { return MD4_Final(md_, &ctx_) == 1; }
  bool Finish() noexcept {
    return MD4_Final(ptr_.underlying_ref(), &ctx_) == 1;
  }

  const uint8_t *data() const noexcept { return ptr_.data(); }

  std::string ToString() const noexcept { return ptr_.ToHexString(); }

 private:
  // Bytes ptr_;
  Bytes ptr_;
  MD4_CTX ctx_;
};

}  // namespace argcv
#endif  // ARGCV_CXX_CRYPTO_MD4_H_
