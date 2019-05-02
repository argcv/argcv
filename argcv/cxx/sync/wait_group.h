// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_SYNC_WAIT_GROUP_H_
#define ARGCV_CXX_SYNC_WAIT_GROUP_H_

#include <cassert>
#include <mutex>  // NOLINT(build/c++11)  std::once_flag

#include "argcv/cxx/base/atomic.h"
#include "argcv/cxx/base/macros.h"
#include "argcv/cxx/base/types.h"

namespace argcv {

namespace sync {

/// Wait Group is a status controller
/// We can define a wait group, and waiting for
/// everything got ready
class WaitGroup {
 public:
  WaitGroup() : state_(0) {}

  virtual ~WaitGroup() {}

  int64 Add(int64 delta = 1) noexcept { return AtomicFetchAdd(&state_, delta); }

  int64 Done() noexcept {
    int64 st = AtomicDecr(&state_);
    assert(st >= 0);
    return st;
  }

  void Wait() const noexcept;

 private:
  atomic<int64> state_;

  DISALLOW_COPY_AND_ASSIGN(WaitGroup);
};

}  // namespace sync

}  // namespace argcv

#endif  // ARGCV_CXX_SYNC_WAIT_GROUP_H_
