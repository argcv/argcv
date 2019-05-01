// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_SYNC_BASE_H_
#define ARGCV_CXX_SYNC_BASE_H_

#include <atomic>
#include <memory>

#include "argcv/cxx/base/types.h"

namespace argcv {

namespace sync {
class LockGuard;

class Locker {
 public:
  virtual void Lock() = 0;
  virtual void Unlock() = 0;

  // is this important?
  // virtual bool TryLock() = 0;

  virtual LockGuard Guard() noexcept;
};

class LockGuard {
 public:
  explicit LockGuard(Locker* lock) : lock_(lock) { lock_->Lock(); }
  ~LockGuard() { lock_->Unlock(); }

 private:
  Locker* lock_;
};

}  // namespace sync

}  // namespace argcv

#endif  //  ARGCV_CXX_SYNC_BASE_H_
