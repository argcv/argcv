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
#ifndef ARGCV_CXX_PAR_DURATION_H_
#define ARGCV_CXX_PAR_DURATION_H_

#include <chrono>  // NOLINT(build/c++11) for std::chrono
#include <map>
#include <string>
#include <thread>  // NOLINT(build/c++11) for Wait
#include <utility>

#include "argcv/cxx/env/macros.h"

namespace argcv {

typedef std::chrono::high_resolution_clock hrclock;
// typedef std::chrono::duration<int64_t, std::ratio<1, 1000000000>> nano;
typedef std::chrono::duration<int64_t, std::nano> nano;
typedef std::chrono::duration<double, std::micro> micro;
typedef std::chrono::duration<double, std::milli> milli;
typedef std::chrono::duration<double> seconds;
typedef std::chrono::duration<double, std::ratio<60>> minutes;
typedef std::chrono::duration<double, std::ratio<60 * 60>> hours;
typedef std::chrono::duration<double, std::ratio<60 * 60 * 24>> days;
typedef std::chrono::time_point<hrclock, nano> time_point;

// using std::chrono::duration_cast;

class Duration {
 public:
  explicit Duration(nano &dur) noexcept : dur_(dur), is_valid_(true) {}

  Duration(const Duration &r) noexcept : dur_(r.dur_), is_valid_(r.is_valid_) {}
  Duration(const Duration &&r)
      : dur_(std::move(r.dur_)), is_valid_(std::move(r.is_valid_)) {}

  static Duration Invalid() noexcept {
    Duration d;
    d.dur_ = nano(0);
    d.is_valid_ = false;
    return d;
  }

  static Duration Zero() noexcept {
    Duration d;
    d.dur_ = nano(0);
    d.is_valid_ = true;
    return d;
  }

  static Duration New(nano dur) noexcept {
    Duration d(dur);
    return d;
  }

  static Duration NewNanoseconds(int64_t value) noexcept {
    nano nano_val(value);
    Duration d(nano_val);
    return d;
  }

  static Duration NewMicroseconds(double value) noexcept {
    nano nano_val(static_cast<int64_t>(value * 1000));
    Duration d(nano_val);
    return d;
  }

  static Duration NewMilliseconds(double value) noexcept {
    nano nano_val(static_cast<int64_t>(value * 1000 * 1000));
    Duration d(nano_val);
    return d;
  }

  static Duration NewSeconds(double value) noexcept {
    nano nano_val(static_cast<int64_t>(value * 1000 * 1000 * 1000));
    Duration d(nano_val);
    return d;
  }

  static Duration NewMinutes(double value) noexcept {
    nano nano_val(static_cast<int64_t>(value * 1000 * 1000 * 1000 * 60));
    Duration d(nano_val);
    return d;
  }

  static Duration NewHours(double value) noexcept {
    nano nano_val(static_cast<int64_t>(value * 1000 * 1000 * 1000 * 60 * 60));
    Duration d(nano_val);
    return d;
  }

  static Duration NewDays(double value) noexcept {
    nano nano_val(
        static_cast<int64_t>(value * 1000 * 1000 * 1000 * 60 * 60 * 24));
    Duration d(nano_val);
    return d;
  }

  // 1/1,000,000,000
  int64_t Nanoseconds() noexcept { return nano(dur_).count(); }

  // 1/1,000,000
  double Microseconds() noexcept { return micro(dur_).count(); }

  // 1/1,000
  double Milliseconds() noexcept { return milli(dur_).count(); }

  // 1
  double Seconds() noexcept { return seconds(dur_).count(); }

  // 60
  double Minutes() noexcept { return minutes(dur_).count(); }

  double Hours() noexcept { return hours(dur_).count(); }

  double Days() noexcept { return days(dur_).count(); }

  double Micro() noexcept { return Microseconds(); }

  // alias
  double Micros() noexcept { return Microseconds(); }

  double Ms() noexcept { return Milliseconds(); }

  double Sec() noexcept { return Seconds(); }

  bool IsValid() noexcept { return is_valid_; }

  bool IsZero() noexcept { return dur_ == nano(0); }

  inline int Compare(const Duration &that) const noexcept {
    return dur_ == that.dur_ ? 0 : (dur_ < that.dur_ ? -1 : +1);
  }

  Duration &operator=(const Duration &that) noexcept {
    dur_ = that.dur_;
    is_valid_ = that.is_valid_;
    return *this;
  }

  Duration operator+(const Duration &that) noexcept {
    bool new_is_valid = is_valid_ && that.is_valid_;
    if (new_is_valid) {
      nano new_dur = dur_ + that.dur_;
      return Duration(new_dur);
    } else {
      return Invalid();
    }
  }

  Duration operator-(const Duration &that) noexcept {
    bool new_is_valid = is_valid_ && that.is_valid_;
    if (new_is_valid) {
      nano new_dur = dur_ - that.dur_;
      return Duration(new_dur);
    } else {
      return Invalid();
    }
  }

  Duration &operator+=(const Duration &that) noexcept {
    is_valid_ = is_valid_ && that.is_valid_;
    if (is_valid_) {
      dur_ += that.dur_;
    } else {
      dur_ = nano(0);
    }
    return *this;
  }

  Duration &operator-=(const Duration &that) noexcept {
    is_valid_ = is_valid_ && that.is_valid_;
    if (is_valid_) {
      dur_ -= that.dur_;
    } else {
      dur_ = nano(0);
    }
    return *this;
  }

  void Wait() const noexcept { std::this_thread::sleep_for(dur_); }

  OP_COMPARABLE(Duration);

 private:
  nano dur_;
  bool is_valid_;
  Duration() {}
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_DURATION_H_
