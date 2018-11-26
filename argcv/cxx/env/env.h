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
#ifndef ARGCV_CXX_ENV_ENV_H_
#define ARGCV_CXX_ENV_ENV_H_

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <string>

#include "argcv/cxx/env/config.h"

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif  // __MACH__

namespace argcv {

class Env {
 public:
  virtual ~Env() = default;
  static Env* Default() {
    static Env env;
    return &env;
  }

  /// \brief Returns the number of nano-seconds since the Unix epoch.
  virtual uint64_t NowNanoseconds() {
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
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000L + ts.tv_nsec;
  }

  /// \brief Returns the number of micro-seconds since the Unix epoch.
  virtual uint64_t NowMicros() { return NowNanoseconds() / 1000L; }

  /// \brief Returns the number of milli-seconds since the Unix epoch.
  virtual uint64_t NowMillis() { return NowNanoseconds() / 1000000L; }

  /// \brief Returns the number of milli-seconds since the Unix epoch.
  virtual uint64_t NowMs() { return NowMillis(); }

  /// \brief Returns the number of seconds since the Unix epoch.
  virtual uint64_t NowSeconds() { return NowNanoseconds() / 1000000000L; }

  /// \brief Get the environment variables by key
  /// Returns true if this variable is valid
  virtual bool GetEnv(const std::string& key, std::string* value) {
    char* v = std::getenv(key.c_str());
    if (v == nullptr || value == nullptr) {
      return false;
    } else {
      *value = v;
      return true;
    }
  }

  virtual size_t GetThreadId();

 private:
};

}  // namespace argcv

#endif  //  ARGCV_CXX_ENV_ENV_H_
