/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================
Original Source: https://goo.gl/qH6C6b
  aka: tensorflow/core/lib/core/status.h@tensorflow

Revised By: Yu Jing
*/

#ifndef ARGCV_CXX_STATUS_STATUS_H_
#define ARGCV_CXX_STATUS_STATUS_H_

#include <functional>
#include <iosfwd>
#include <string>

#include "argcv/cxx/str/str.h"
#include "argcv/proto/status/error_codes.pb.h"

#include "glog/logging.h"

namespace argcv {

using ECode = error::Code;
// using ::argcv::StrSlice;

class Status {
 public:
  /// Create a success status.
  Status() : state_(NULL) {}
  ~Status() { delete state_; }

  /// \brief Create a status with the specified error code and msg as a
  /// human-readable string containing more detailed information.
  Status(ECode code, const std::string& msg);

  /// Copy the specified status.
  Status(const Status& s);
  void operator=(const Status& s);

  static Status OK() { return Status(); }

  /// Returns true iff the status indicates success.
  bool ok() const { return (state_ == NULL); }

  ECode code() const { return ok() ? error::OK : state_->code; }

  const std::string& error_message() const {
    return ok() ? empty_string() : state_->msg;
  }

  bool operator==(const Status& x) const;
  bool operator!=(const Status& x) const;

  /// \brief If `ok()`, stores `new_status` into `*this`.  If `!ok()`,
  /// preserves the current status, but may augment with additional
  /// information about `new_status`.
  ///
  /// Convenient way of keeping track of the first error encountered.
  /// Instead of:
  ///   `if (overall_status.ok()) overall_status = new_status`
  /// Use:
  ///   `overall_status.Update(new_status);`
  void Update(const Status& new_status);

  /// \brief Return a string representation of this status suitable for
  /// printing. Returns the string `"OK"` for success.
  std::string ToString() const;

 private:
  static const std::string& empty_string();
  struct State {
    ECode code;
    std::string msg;
  };
  // OK status has a `NULL` state_.  Otherwise, `state_` points to
  // a `State` structure containing the error code and message(s)
  State* state_;

  void SlowCopyFrom(const State* src);
};

inline Status::Status(const Status& s)
    : state_((s.state_ == NULL) ? NULL : new State(*s.state_)) {}

inline void Status::operator=(const Status& s) {
  // The following condition catches both aliasing (when this == &s),
  // and the common case where both s and *this are ok.
  if (state_ != s.state_) {
    SlowCopyFrom(s.state_);
  }
}

inline bool Status::operator==(const Status& x) const {
  return (this->state_ == x.state_) || (ToString() == x.ToString());
}

inline bool Status::operator!=(const Status& x) const { return !(*this == x); }

std::ostream& operator<<(std::ostream& os, const Status& x);

typedef std::function<void(const Status&)> StatusCallback;

#define CHECK_OK(val) CHECK_EQ(::argcv::Status::OK(), (val))
#define QCHECK_OK(val) QCHECK_EQ(::argcv::Status::OK(), (val))

}  // namespace argcv

#endif  // ARGCV_CXX_STATUS_STATUS_H_
