// Copyright 2021 Yu Jing
#include "argcv/sync/wait_group.h"

#include "argcv/base/atomic.h"
#include "argcv/base/concurrency.h"

namespace argcv {
namespace sync {
void WaitGroup::Wait() const noexcept {
  while (AtomicLoad(&state_) > 0) {
    Yield();
  }
}
}  // namespace sync
}  // namespace argcv
