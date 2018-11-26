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
Original Source: https://goo.gl/rzZZtW
  aka: tensorflow/core/lib/core/errors.h@tensorflow

Revised By: Yu Jing
*/
#ifndef ARGCV_CXX_STATUS_ERRORS_H_
#define ARGCV_CXX_STATUS_ERRORS_H_

#include "argcv/cxx/env/macros.h"
#include "argcv/cxx/status/status.h"
#include "argcv/cxx/str/str_slice.h"
#include "argcv/proto/status/error_codes.pb.h"

namespace argcv {
namespace errors {

// typedef ::argcv::Code Code;
using ::argcv::StrSlice;

// Append some context to an error message.  Each time we append
// context put it on a new line, since it is possible for there
// to be several layers of additional context.
template <typename... Args>
void AppendToMessage(::argcv::Status* status, Args... args) {
  *status = ::argcv::Status(
      status->code(),
      StrSlice(status->error_message()).Append("\n\t", args...).ToString());
}

// For propagating errors when calling a function.
#define RETURN_IF_ERROR(expr)                         \
  do {                                                \
    const ::argcv::Status _status = (expr);           \
    if (PREDICT_FALSE(!_status.ok())) return _status; \
  } while (0)

#define RETURN_WITH_CONTEXT_IF_ERROR(expr, ...)                \
  do {                                                         \
    ::argcv::Status _status = (expr);                          \
    if (PREDICT_FALSE(!_status.ok())) {                        \
      ::argcv::errors::AppendToMessage(&_status, __VA_ARGS__); \
      return _status;                                          \
    }                                                          \
  } while (0)

// Convenience functions for generating and using error status.
// Example usage:
//   status.Update(errors::InvalidArgument("The ", foo, " isn't right."));
//   if (errors::IsInvalidArgument(status)) { ... }
//   switch (status.code()) { case error::INVALID_ARGUMENT: ... }

#define DECLARE_ERROR(FUNC, CONST)                                 \
  template <typename... Args>                                      \
  ::argcv::Status FUNC(Args... args) {                             \
    return ::argcv::Status(::argcv::error::CONST,                  \
                           StrSlice().Append(args...).ToString()); \
  }                                                                \
  inline bool Is##FUNC(const ::argcv::Status& status) {            \
    return status.code() == ::argcv::error::CONST;                 \
  }

DECLARE_ERROR(Cancelled, CANCELLED)
DECLARE_ERROR(InvalidArgument, INVALID_ARGUMENT)
DECLARE_ERROR(NotFound, NOT_FOUND)
DECLARE_ERROR(AlreadyExists, ALREADY_EXISTS)
DECLARE_ERROR(ResourceExhausted, RESOURCE_EXHAUSTED)
DECLARE_ERROR(Unavailable, UNAVAILABLE)
DECLARE_ERROR(FailedPrecondition, FAILED_PRECONDITION)
DECLARE_ERROR(OutOfRange, OUT_OF_RANGE)
DECLARE_ERROR(Unimplemented, UNIMPLEMENTED)
DECLARE_ERROR(Internal, INTERNAL)
DECLARE_ERROR(Aborted, ABORTED)
DECLARE_ERROR(DeadlineExceeded, DEADLINE_EXCEEDED)
DECLARE_ERROR(DataLoss, DATA_LOSS)
DECLARE_ERROR(Unknown, UNKNOWN)
DECLARE_ERROR(PermissionDenied, PERMISSION_DENIED)
DECLARE_ERROR(Unauthenticated, UNAUTHENTICATED)

#undef DECLARE_ERROR

// The CanonicalCode() for non-errors.
using ::argcv::error::OK;

}  // namespace errors

}  // namespace argcv

#endif  // ARGCV_CXX_STATUS_ERRORS_H_
