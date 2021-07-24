// Copyright 2021 Yu Jing
#ifndef ARGCV_SYNC_WAIT_GROUP_H_
#define ARGCV_SYNC_WAIT_GROUP_H_

#include <cassert>
#include <condition_variable>
#include <mutex>  // NOLINT(build/c++11)  std::once_flag

#include "argcv/base/atomic.h"
#include "argcv/base/macros.h"
#include "argcv/base/types.h"

namespace argcv {

namespace sync {

/// Wait Group is a status controller
/// We can define a wait group, and waiting for
/// everything got ready
class WaitGroup {
 public:
  WaitGroup() : state_(0) {}

  virtual ~WaitGroup() { cv_.notify_all(); }

  int64 Add(int64 delta = 1) noexcept { return AtomicFetchAdd(&state_, delta); }

  int64 Done() noexcept {
    int64 st = AtomicDecr(&state_);
    assert(st >= 0);
    cv_.notify_all();
    return st;
  }

  void Wait() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return AtomicLoad(&this->state_) == 0; });
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(WaitGroup);

  atomic<int64> state_;
  std::mutex mutex_;
  std::condition_variable cv_;
};

}  // namespace sync

}  // namespace argcv

#endif  // ARGCV_SYNC_WAIT_GROUP_H_
