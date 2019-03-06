// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_STATUS_H_
#define ARGCV_CXX_BASE_STATUS_H_

#include <string.h>  // memcpy

#include <map>

#include "absl/strings/str_cat.h"
#include "argcv/cxx/base/code.h"
#include "argcv/cxx/base/types.h"

#include "glog/logging.h"

namespace argcv {

using ECode = error::Code;

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

/**
 * \brief A generic return type
 *
 * There are a few ways to return a Status with error message.
 * In this project, we are supposed to create a global type, contains
 * Successful information with error message instead of throwing a exception.
 */
class Status {
 public:
  Status() noexcept : state_(nullptr) {}
  ~Status() { delete[] state_; }

  Status(const Status& rhs) noexcept {
    state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
  }

  /// copy
  Status& operator=(const Status& rhs) noexcept {
    /// The following condition catches both aliasing (when this == &rhs),
    /// and the common case where both rhs and *this are ok.
    if (state_ != rhs.state_) {
      delete[] state_;
      state_ = (rhs.state_ == nullptr) ? nullptr : CopyState(rhs.state_);
    }
    return *this;
  }

  /// swap
  Status& operator=(Status&& rhs) noexcept {
    std::swap(state_, rhs.state_);
    return *this;
  }

  Status(Status&& rhs) noexcept : state_(rhs.state_) { rhs.state_ = nullptr; }

  /// Returns true if the status indicates success.
  bool ok() const noexcept { return (state_ == nullptr); }

  /// Return status code
  ECode code() const noexcept {
    return (state_ == nullptr) ? ::argcv::error::kOk
                               : static_cast<ECode>(state_[4]);
  }

  /// Return error message
  const string_view error_message() const noexcept {
    if (ok()) {
      return empty_string();
    } else {
      // std::string result;
      // uint32_t length;
      // memcpy(&length, state_, sizeof(length));
      // result.append(state_ + 5, length);
      // return result;
      uint32_t length;
      memcpy(&length, state_, sizeof(length));
      return string_view(state_ + 5, length);
    }
  }

  bool operator==(const Status& x) const noexcept {
    return (this->state_ == x.state_) || (ToString() == x.ToString());
  }
  bool operator!=(const Status& x) const noexcept { return !(*this == x); }

  /// Return a string representation of this status suitable for printing.
  /// Returns the string "OK" for success.
  std::string ToString() const noexcept {
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

  template <typename... Args>
  Status& AppendToMessage(Args... args) noexcept {
    ECode new_code = code();
    if (ok()) {
      new_code = ::argcv::error::kUnknown;
    }
    std::string msg = absl::StrCat(error_message(), "\n\t", args...);
    const uint32_t size = msg.size();
    char* result = new char[size + 5];
    memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(new_code);
    memcpy(result + 5, msg.data(), size);
    std::swap(state_, result);
    return *this;
  }

  /// Return a success status.
  ///
  /// usage: Status st_ok = Status::OK();
  static Status OK() noexcept { return Status(); }

#define DECLARE_ERROR_FUNC(FUNC)                                   \
  /*! \brief Return a `FUNC` status with messages */               \
  template <typename... Args>                                      \
  static Status FUNC(Args... args) noexcept {                      \
    return Status(::argcv::error::k##FUNC, absl::StrCat(args...)); \
  }                                                                \
                                                                   \
  /*! \brief Returns true if the error code is `FUNC` */           \
  bool Is##FUNC() const noexcept { return code() == ::argcv::error::k##FUNC; }

  DECLARE_EACH_STATUS_CODE(DECLARE_ERROR_FUNC)

#undef DECLARE_ERROR_FUNC  // DECLARE_ERROR_FUNC(FUNC)

 private:
  // OK status has a null state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  char* state_;
  // const char* state_;
  const static std::map<int, std::string> code_desc_;
  const static std::string& empty_string();

  Status(ECode code, const string_view& msg) noexcept {
    assert(code != ::argcv::error::kOk);
    const uint32_t size = msg.size();
    char* result = new char[size + 5];
    memcpy(result, &size, sizeof(size));
    result[4] = static_cast<char>(code);
    memcpy(result + 5, msg.data(), size);
    state_ = result;
  }

  static char* CopyState(const char* state) noexcept {
    uint32_t size;
    memcpy(&size, state, sizeof(size));
    char* result = new char[size + 5];
    memcpy(result, state, size + 5);
    return result;
  }
};

const std::map<int, std::string> Status::code_desc_{
#define DECLARE_CODE_DESC(ELEM) {::argcv::error::k##ELEM, #ELEM},
    DECLARE_EACH_STATUS_CODE(DECLARE_CODE_DESC)
#undef DECLARE_CODE_DESC
};

std::ostream& operator<<(std::ostream& os, const Status& x);

// is it required?
// typedef std::function<void(const Status&)> StatusCallback;

/// see: http://rpg.ifi.uzh.ch/docs/glog.html#check
#define CHECK_OK(val) CHECK_EQ(::argcv::Status::OK(), (val))

// template <typename T>
// class Result {
//  public:

//  private:
// };

#undef DECLARE_EACH_STATUS_CODE

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_STATUS_H_
