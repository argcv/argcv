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
#ifndef ARGCV_CXX_PAR_TICKER_H_
#define ARGCV_CXX_PAR_TICKER_H_

#include <functional>
#include <map>
#include <memory>
#include <mutex>  // NOLINT(build/c++11)
#include <string>
#include <utility>

#include "argcv/cxx/par/duration.h"
#include "argcv/cxx/par/thread_pool.h"

namespace argcv {

class Ticker {
 public:
  explicit Ticker(int reserve_threads = 0) noexcept
      : shutdown_(false), pool_(new ThreadPool(reserve_threads)) {}

  ~Ticker() {
    shutdown_ = true;
    if (pool_) {
      delete pool_;
    }
  }

  bool Sched(const std::function<bool()>& func, Duration dur) {
    if (pool_) {
      pool_->Send([=] {
        while (!(this->shutdown_) && func()) {
          dur.Wait();
        }
      });
      return true;
    } else {
      return false;
    }
  }

  bool Join() {
    if (pool_) {
      delete pool_;
      pool_ = nullptr;
      return true;
    } else {
      return false;
    }
  }

  void Stop() { shutdown_ = true; }

 private:
  bool shutdown_;
  ThreadPool* pool_;
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_TICKER_H_
