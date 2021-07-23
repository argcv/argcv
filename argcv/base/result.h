// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_RESULT_H_
#define ARGCV_BASE_RESULT_H_

#include <utility>  // move

#include "argcv/base/status.h"

namespace argcv {

template <typename T>
class Result {
 public:
  Result(Status status, const T& v) noexcept : status_(status), r_(v) {}

  virtual ~Result() {}
  // copy
  Result(const Result& rhs) noexcept : status_(rhs.status_), r_(rhs.r_) {}

  // swap
  Result(Result&& rhs) noexcept : status_(std::move(rhs.status_)), r_(rhs.r_) {}

  /// copy
  Result& operator=(const Result& rhs) noexcept {
    status_ = rhs.status_;
    r_ = rhs.r_;
    return *this;
  }

  /// swap
  Result& operator=(Result&& rhs) noexcept {
    status_ = rhs.status_;
    r_ = rhs.r_;
    return *this;
  }

  Status status() const noexcept { return status_; }

  bool ok() const noexcept { return status_.ok(); }

  T data() const noexcept { return r_; }

 private:
  Status status_;
  T r_;
  Result() = delete;
};

}  // namespace argcv

#endif  // ARGCV_BASE_RESULT_H_
