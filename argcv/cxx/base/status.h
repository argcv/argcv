// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_STATUS_H_
#define ARGCV_CXX_BASE_STATUS_H_

#include <string.h>  // memcpy

#include <map>

#include "absl/strings/str_cat.h"
#include "argcv/cxx/base/code.h"
#include "argcv/cxx/base/types.h"

namespace argcv {

#define DECLARE_EACH_STATUS_CODE(MACRO_FUNC) \
  MACRO_FUNC(Cancelled)                      \
  MACRO_FUNC(InvalidArgument)                \
  MACRO_FUNC(NotFound)                       \
  MACRO_FUNC(AlreadyExists)                  \
  MACRO_FUNC(ResourceExhausted)              \
  MACRO_FUNC(Unavailable)                    \
  MACRO_FUNC(FailedPrecondition)             \
  MACRO_FUNC(OutOfRange)                     \
  MACRO_FUNC(Unimplemented)                  \
  MACRO_FUNC(Internal)                       \
  MACRO_FUNC(Aborted)                        \
  MACRO_FUNC(DeadlineExceeded)               \
  MACRO_FUNC(DataLoss)                       \
  MACRO_FUNC(Unknown)                        \
  MACRO_FUNC(PermissionDenied)               \
  MACRO_FUNC(Unauthenticated)

class Status {
 public:
  Status() noexcept : state_(nullptr) {}
  ~Status() { delete[] state_; }

  Status(const Status& rhs) {
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }

  Status& operator=(const Status& rhs) {
    // The following condition catches both aliasing (when this == &rhs),
    // and the common case where both rhs and *this are ok.
    if (state_ != rhs.state_) {
      delete[] state_;
      state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
  }

  Status& operator=(Status&& rhs) noexcept {
    std::swap(state_, rhs.state_);
    return *this;
  }

  Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }

  // Returns true iff the status indicates success.
  bool ok() const { return (state_ == nullptr); }

  // Return status code
  Code code() const {
    return (state_ == nullptr) ? kOk : static_cast<Code>(state_[4]);
  }

  // Return a string representation of this status suitable for printing.
  // Returns the string "OK" for success.
  std::string ToString() const {
    if (state_ == nullptr) {
      return "OK";
    } else {
      char tmp[30];
      const char* type;
      int icode = static_cast<int>(code());
      auto it = code_desc_.find(icode);
      if (it != code_desc_.end()) {
        snprintf(tmp, sizeof(tmp), "Error %s (%d): ", it->second.c_str(),
                 icode);
      } else {
        snprintf(tmp, sizeof(tmp), "Unknown Error (%d): ", icode);
      }
      type = tmp;
      std::string result(type);
      uint32_t length;
      memcpy(&length, state_, sizeof(length));
      result.append(state_ + 5, length);
      return result;
    }
  }

  // Return a success status.
  static Status OK() { return Status(); }

#define DECLARE_ERROR_FUNC(FUNC)                   \
  template <typename... Args>                      \
  static Status FUNC(Args... args) {               \
    return Status(k##FUNC, absl::StrCat(args...)); \
  }                                                \
  bool Is##FUNC() const { return code() == k##FUNC; }

  DECLARE_EACH_STATUS_CODE(DECLARE_ERROR_FUNC)

#undef DECLARE_ERROR_FUNC  // DECLARE_ERROR_FUNC(FUNC)

 private:
  // OK status has a null state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;
  const static std::map<int, std::string> code_desc_;

  Status(Code code, const string_view& msg) {
    assert(code != kOk);
    const uint32_t size = msg.size();
    char* result = new char[size + 5];
    memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), size);
    state_ = result;
  }

  static const char* CopyState(const char* state) {
    uint32_t size;
    memcpy(&size, state, sizeof(size));
    char* result = new char[size + 5];
    memcpy(result, state, size + 5);
    return result;
  }
};

const std::map<int, std::string> Status::code_desc_{
#define DECLARE_CODE_DESC(ELEM) {k##ELEM, #ELEM},
    DECLARE_EACH_STATUS_CODE(DECLARE_CODE_DESC)
#undef DECLARE_CODE_DESC
};

#undef DECLARE_EACH_STATUS_CODE

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_STATUS_H_
