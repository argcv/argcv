// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/wait_group.h"

#include "argcv/cxx/base/atomic.h"
#include "argcv/cxx/base/concurrency.h"

namespace argcv {
namespace sync {
void WaitGroup::Wait() const noexcept {
  while (AtomicLoad(&state_) > 0) {
    Yield();
  }
}
}  // namespace sync
}  // namespace argcv
