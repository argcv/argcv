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
#ifndef ARGCV_CXX_PAR_TIMER_H_
#define ARGCV_CXX_PAR_TIMER_H_

#include <chrono>  // NOLINT(build/c++11) for std::chrono
#include <map>
#include <string>
#include <utility>

#include "argcv/cxx/par/duration.h"

namespace argcv {

class Timer {
 public:
  Timer() noexcept : tp_begin_(hrclock::now()) {}

  ~Timer() {}

  void Label(const std::string& k) noexcept {
    tp_labels_.insert(std::make_pair(k, hrclock::now()));
  }

  // start to now
  Duration All() noexcept {
    return Duration::New((hrclock::now() - tp_begin_));
  }

  Duration From(const std::string& k) noexcept {
    std::map<std::string, time_point>::const_iterator p = tp_labels_.find(k);
    return p == tp_labels_.end() ? Duration::Invalid()
                                 : Duration::New((hrclock::now() - p->second));
  }

  Duration To(const std::string& k) noexcept {
    std::map<std::string, time_point>::const_iterator p = tp_labels_.find(k);
    return p == tp_labels_.end() ? Duration::Invalid()
                                 : Duration::New((p->second - tp_begin_));
  }

  Duration Until(const std::string& k) noexcept { return To(k); }

  Duration Between(const std::string& t, const std::string& f) noexcept {
    std::map<std::string, time_point>::const_iterator p0 = tp_labels_.find(t);
    std::map<std::string, time_point>::const_iterator p1 = tp_labels_.find(f);
    return p0 == tp_labels_.end() || p1 == tp_labels_.end()
               ? Duration::Invalid()
               : Duration::New(p0->second > p1->second
                                   ? p0->second - p1->second
                                   : p1->second - p0->second);
  }

 protected:
  time_point tp_begin_;  // start point
  std::map<std::string, time_point> tp_labels_;
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_TIMER_H_
