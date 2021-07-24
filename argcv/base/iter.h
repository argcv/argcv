// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_ITER_H_
#define ARGCV_BASE_ITER_H_

#include <memory>

#include "argcv/base/macros.h"
#include "argcv/base/types.h"
#include "glog/logging.h"

namespace argcv {

/// \brief a iterable element
template <typename T>
struct Iterable {
  Iterable() {}
  virtual ~Iterable() {}
  virtual bool Next(T& item) = 0;
};

template <typename T>
class Iterator {
 public:
  explicit Iterator() : it_(nullptr), underlying_ref_(nullptr), eof_(true) {}
  explicit Iterator(Iterable<T>* it)
      : it_(it),
        underlying_ref_(static_cast<T*>(std::malloc(sizeof(T)))),
        eof_(it == nullptr) {}

  // each 'copy' may re-allocate a new underlying_ref_
  // this means all the state may lost, a new 'begin' may required
  Iterator(const Iterator& rhs)
      : it_(rhs.it_),
        underlying_ref_(static_cast<T*>(std::malloc(sizeof(T)))),
        eof_(rhs.it_ == nullptr) {}

  Iterator& operator=(const Iterator& rhs) = delete;

  /// swap
  Iterator& operator=(Iterator&& rhs) noexcept {
    LOG(INFO) << "SWAP...";
    std::swap(it_, rhs.it_);
    std::swap(underlying_ref_, rhs.underlying_ref_);
    std::swap(eof_, rhs.eof_);
    return *this;
  }

  /// swap
  Iterator(Iterator&& rhs) noexcept
      : it_(rhs.it_), underlying_ref_(rhs.underlying_ref_), eof_(rhs.eof_) {
    LOG(INFO) << "SWAP2...";
    rhs.it_ = nullptr;
    rhs.underlying_ref_ = nullptr;
    rhs.eof_ = true;
  }

  virtual ~Iterator() {
    if (underlying_ref_ != nullptr) {
      delete underlying_ref_;
      underlying_ref_ = nullptr;
    }
  }

  //////////////////////////////////////////////
  Iterator<T>& begin() {
    // LOG(INFO) << "begin";
    if (it_ == nullptr) {
      eof_ = true;
      return *this;
    }
    eof_ = !it_->Next(*underlying_ref_);
    // LOG(INFO) << "begin result: " << *underlying_ref_ << ", eof:" << eof_;
    return *this;
  }

  Iterator<T> end() {
    // LOG(INFO) << "end";
    return Iterator<T>(nullptr);
  }

  T& operator*() const {
    // LOG(INFO) << "operator*";
    return *underlying_ref_;
  }

  Iterator<T>& operator++() {
    // LOG(INFO) << "Next...";
    eof_ = !it_->Next(*underlying_ref_);
    // LOG(INFO) << "Next result: " << *underlying_ref_ << ", eof:" << eof_;
    return *this;
  }

  bool operator!=(const Iterator<T>& rhs) const {
    // LOG(INFO) << "EOF ?" << eof_ << "RHS_EOF ?" << rhs.eof_;
    return eof_ != rhs.eof_;
  }

 private:
  Iterable<T>* it_;
  T* underlying_ref_;
  bool eof_;
};

}  // namespace argcv

#endif  // ARGCV_BASE_ITER_H_
