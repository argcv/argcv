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
#ifndef ARGCV_CXX_PAR_UUID_H_
#define ARGCV_CXX_PAR_UUID_H_

#include <inttypes.h>
#include <pthread.h>
#include <sys/time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif  // __MACH__

#include <cstring>  //  memset
#include <ctime>

#include <string>
#include <utility>

#include "argcv/cxx/env/macros.h"
#include "argcv/cxx/math/rand.h"
#include "argcv/cxx/type/byte.h"

namespace argcv {

const size_t kSzUuidHexBuff = sizeof(uint64_t) * 4 + 1;  //  8 * 4 + 1 = 33

// rel: https://tools.ietf.org/html/rfc4122
// rel: http://docs.oracle.com/javase/7/docs/api/java/util/UUID.html
// rel:
// https://support.datastax.com/hc/en-us/articles/204226019-Converting-TimeUUID-Strings-to-Dates
class Uuid {
 public:
  explicit Uuid(uint16_t node = 0) { assign(node); }
  explicit Uuid(uint64_t hi, uint64_t lo) : hi_(hi), lo_(lo) {}
  explicit Uuid(const std::string& str) {
    hi_ = 0;
    lo_ = 0;
    if (str.size() == 36 && str[8] == '-' && str[13] == '-' && str[18] == '-' &&
        str[23] == '-') {
      // std::cout << "is string !!!!" << std::endl;
      // string
      for (size_t i = 0; i < 18; i++) {
        if (str[i] >= 'a' && str[i] <= 'f') {
          hi_ = hi_ << 4 | (str[i] - 'a' + 10);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
          hi_ = hi_ << 4 | (str[i] - 'A' + 10);
        } else if (str[i] >= '0' && str[i] <= '9') {
          hi_ = hi_ << 4 | (str[i] - '0');
        }
      }
      for (size_t i = 18; i < 36; i++) {
        if (str[i] >= 'a' && str[i] <= 'f') {
          lo_ = lo_ << 4 | (str[i] - 'a' + 10);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
          lo_ = lo_ << 4 | (str[i] - 'A' + 10);
        } else if (str[i] >= '0' && str[i] <= '9') {
          lo_ = lo_ << 4 | (str[i] - '0');
        }
      }
    } else if (str.size() == 32) {
      // std::cout << "is hex!!" << std::endl;
      // hex
      for (size_t i = 0; i < 16; i++) {
        if (str[i] >= 'a' && str[i] <= 'f') {
          hi_ = hi_ << 4 | (str[i] - 'a' + 10);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
          hi_ = hi_ << 4 | (str[i] - 'A' + 10);
        } else if (str[i] >= '0' && str[i] <= '9') {
          hi_ = hi_ << 4 | (str[i] - '0');
        }
      }
      for (size_t i = 16; i < 32; i++) {
        if (str[i] >= 'a' && str[i] <= 'f') {
          lo_ = lo_ << 4 | (str[i] - 'a' + 10);
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
          lo_ = lo_ << 4 | (str[i] - 'A' + 10);
        } else if (str[i] >= '0' && str[i] <= '9') {
          lo_ = lo_ << 4 | (str[i] - '0');
        }
      }
    } else if (str.size() == sizeof(uint64_t) * 2) {
      // std::cout << "is data!!!" << std::endl;
      // binary
      size_t sz = sizeof(uint64_t);
      Bytes ba(str);
      ba.Write<uint64_t>(&hi_, 0);
      ba.Write<uint64_t>(&lo_, sz);
      // LoadBytes<uint64_t>(&hi_, str, 0);
      // LoadBytes<uint64_t>(&lo_, str, sz);
    }
  }

  bool assign(uint16_t node = 0) {
    uint64_t seed = uuid_ts();

    uint32_t timelo_w = seed & 0xFFFFFFFF;
    uint16_t time_mid = (seed >> 32) & 0xFFFF;
    uint16_t timehi__version = (seed >> 48) & 0xFFF;

    memset(&hi_, 0x0, sizeof(hi_));
    memset(&lo_, 0x0, sizeof(lo_));

    // version = 1 , ref: rfc4122
    hi_ = (uint64_t)timelo_w << 32 | (uint64_t)time_mid << 16 |
          (uint64_t)timehi__version;

    // set version
    hi_ &= ~0xF000;
    hi_ |= _version << 12;

    // lo_
    uint16_t clock_seq = argcv::RandomInt() & 0x3FFF;  //
    // uint8_t clock_seqlo_w = clock_seq & 0xFF;
    // uint8_t clock_seqhi__variant = (clock_seq >> 8) & 0x3F;

    lo_ = (uint64_t)clock_seq << 48 | node;

    // variant = 2  ref: rfc4122
    lo_ &= ~((uint64_t)0xC000 << 48);
    lo_ |= (uint64_t)0x8000 << 48;

    return true;
  }

  ~Uuid() {}

  std::string Hex() {
    // char buff[kSzUuidHexBuff];
    // snprintf(buff, kSzUuidHexBuff, "%016" PRIx64 "%016" PRIx64 "", hi_, lo_);
    // return std::string(buff);
    // ToBytes(d).ToHexString();
    // std::string buff = ToBytes(hi_);
    // buff += ToBytes(lo_);
    return ToBytes({hi_, lo_}).ToHexString();
  }

  std::string data() { return ToBytes<uint64_t>({hi_, lo_}).ToString(); }

  std::string ToString() {
    std::string s_val;
    std::string s_hex = Hex();
    for (int i = 0; i < 8; i++) {
      s_val += s_hex[i];
    }
    s_val += '-';
    for (int i = 8; i < 12; i++) {
      s_val += s_hex[i];
    }
    s_val += '-';
    for (int i = 12; i < 16; i++) {
      s_val += s_hex[i];
    }
    s_val += '-';
    for (int i = 16; i < 20; i++) {
      s_val += s_hex[i];
    }
    s_val += '-';
    for (int i = 20; i < 32; i++) {
      s_val += s_hex[i];
    }
    return s_val;
  }

  std::pair<uint64_t, uint64_t> Pair() { return std::make_pair(hi_, lo_); }

  inline int Compare(const Uuid& that) const noexcept {
    if (hi_ == that.hi_) {
      // eq?
      return lo_ == that.lo_ ? 0 : (lo_ < that.lo_ ? -1 : +1);
    } else {
      return hi_ < that.hi_ ? -1 : +1;
    }
    // return r != 0 ? r : (sz == that_sz ? 0 : (sz < that_sz ? -1 : +1));
  }

  OP_COMPARABLE(Uuid);

 private:
  uint64_t hi_;
  uint64_t lo_;

  static const uint64_t _version = 1;  //  uuid version

  inline uint64_t uuid_ts() {
    static uint64_t lts = 0;
    struct timespec ts;
#ifdef __MACH__
    // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif  // __MACH__

    uint64_t cts =
        ts.tv_sec * 10000000 + (ts.tv_nsec / 100) + 0x01b21dd213814000;
    if (cts <= lts) {
      cts = ++lts;
    } else {
      lts = cts;
    }
    return cts;
  }
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_UUID_H_
