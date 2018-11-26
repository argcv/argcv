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
#ifndef ARGCV_CXX_CRYPTO_SHA512_H_
#define ARGCV_CXX_CRYPTO_SHA512_H_

#include <openssl/sha.h>

#include <cstdio>
#include <string>

#include "argcv/cxx/crypto/base.h"
#include "argcv/cxx/type/byte.h"

namespace argcv {

class SHA512Helper : public BaseHasher {
 public:
  SHA512Helper() noexcept {
    ptr_.Resize(SHA512_DIGEST_LENGTH);
    SHA512_Init(&ctx_);
  }

  virtual ~SHA512Helper() {
    // delete[] md_;
  }

  bool Update(const std::string &str) noexcept {
    return SHA512_Update(&ctx_, static_cast<const void *>(str.data()),
                         str.length()) == 1;
  }

  bool Finish() noexcept {
    return SHA512_Final(ptr_.underlying_ref(), &ctx_) == 1;
  }

  const uint8_t *data() const noexcept { return ptr_.data(); }

  // std::string ToString() const noexcept {
  //   char buffer[2 * SHA512_DIGEST_LENGTH + 1];
  //   for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
  //     snprintf(&buffer[i * 2], 3, "%02x",  // NOLINT(runtime/printf)
  //              static_cast<unsigned int>(md_[i]));
  //   }
  //   buffer[SHA512_DIGEST_LENGTH * 2] = '\0';
  //   return std::string(buffer, 2 * SHA512_DIGEST_LENGTH);
  // }

  std::string ToString() const noexcept { return ptr_.ToHexString(); }

 private:
  Bytes ptr_;
  SHA512_CTX ctx_;
};

}  // namespace argcv
#endif  // ARGCV_CXX_CRYPTO_SHA512_H_
