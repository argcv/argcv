// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_SYNC_MUTEX_H_
#define ARGCV_CXX_SYNC_MUTEX_H_

#include <atomic>
#include <memory>

#include "argcv/cxx/base/atomic.h"
#include "argcv/cxx/base/macros.h"
#include "argcv/cxx/base/types.h"
#include "argcv/cxx/sync/base.h"

namespace argcv {

namespace sync {

/// \brief A implementation of spinlock
class Mutex : public Locker {
 public:
  // ATOMIC_FLAG_INIT = {false}
  // Mutex() noexcept : locked_(std::make_shared<std::atomic_flag>()) {}
  Mutex() noexcept {
    // or locked_ = ATOMIC_FLAG_INIT;
    locked_.clear(std::memory_order_release);
  }

  void Lock() noexcept;

  bool TryLock() noexcept {
    return locked_.test_and_set(std::memory_order_acquire);
  }

  void Unlock() noexcept {
    // unlock, always clear the flag
    locked_.clear(std::memory_order_release);
  }

 private:
  std::atomic_flag locked_;
  DISALLOW_COPY_AND_ASSIGN(Mutex);
};

}  // namespace sync

}  // namespace argcv

#endif  // ARGCV_CXX_SYNC_MUTEX_H_
