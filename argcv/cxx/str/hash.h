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
#ifndef ARGCV_CXX_STR_HASH_H_
#define ARGCV_CXX_STR_HASH_H_

#include <string>

namespace argcv {

// a hasher, from string to uint64_t
// usage :
// argcv::string::BlzHasher h;
// printf("%llu\n",h.hash("hello",0));
class BlzHasher {
 public:
  BlzHasher() { Init(); }
  ~BlzHasher() {}

  // k : string , offset shall not too large , 0-3 in suggestion
  uint64_t Hash(const std::string &k, uint16_t offset = 0) {
    uint64_t seed_a = 0x7FED7FED, seed_b = 0xEEEEEEEE;
    uint64_t ch;
    for (uint64_t i = 0; i < k.length(); i++) {
      ch = (uint64_t)k[i];
      // ch = toupper(*k++);
      seed_a = crypt_[(offset << 8) + ch] ^ (seed_a + seed_b);
      seed_b = ch + seed_a + seed_b + (seed_b << 5) + 3;
    }
    return seed_a;
  }

 private:
  uint64_t crypt_[0x500];  // seed

  bool Init() {
    uint64_t seed = 0x00100001, idx_a = 0, idx_b = 0, i;
    for (idx_a = 0; idx_a < 0x100; idx_a++) {
      for (idx_b = idx_a, i = 0; i < 5; i++, idx_b += 0x100) {
        uint64_t t1, t2;
        seed = (seed * 125 + 3) % 0x2AAAAB;
        t1 = (seed & 0xFFFF) << 0x10;
        seed = (seed * 125 + 3) % 0x2AAAAB;
        t2 = (seed & 0xFFFF);
        crypt_[idx_b] = (t1 | t2);
      }
    }
    return true;
  }
};

}  // namespace argcv

#endif  //  ARGCV_CXX_STR_HASH_H_
