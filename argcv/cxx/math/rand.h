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
#ifndef ARGCV_CXX_MATH_RAND_H_
#define ARGCV_CXX_MATH_RAND_H_

#include <cstdlib>
#include <ctime>

#include <string>

namespace argcv {

/**
 * Some Helper for generate random number/strings
 * Note:
 * http://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
 * here is a better sulution here
 */

static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

/**
 * Random Int is only guaranteed 15 random bits
 */
inline int RandomInt() {
  static unsigned int seed = (unsigned int)time(NULL);
  return rand_r(&seed);
}

/**
 *
 */
inline uint64_t RamdomUInt64() {
  uint64_t ret = 0;
  for (int i = 0; i < 5; i++) {
    // RANDOM MAX = 111 1111 1111 1111
    ret = (ret << 15) | (RandomInt() & 0x7FFF);
  }
  return ret;
}

const uint64_t kRamdomUInt64Max = 0xffffffffffffffff;

inline double RandomDouble() {
  return static_cast<double>(RandomInt()) / RAND_MAX;
}

inline double RamdonUInt64Double() {
  return static_cast<double>(RamdomUInt64()) / kRamdomUInt64Max;
}

inline std::string RandomStr(const int len, const std::string& cands) {
  size_t cdlen = cands.size();
  std::string s;
  for (int i = 0; i < len; ++i) {
    s += cands[RandomInt() % cdlen];
  }
  return s;
}

}  // namespace argcv

#endif  //  ARGCV_CXX_MATH_RAND_H_
