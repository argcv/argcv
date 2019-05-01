// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/wait_group.h"

#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

#include "argcv/cxx/base/atomic.h"

namespace argcv {
namespace sync {
void WaitGroup::Wait() const noexcept {
  while (AtomicLoad(&state_) > 0) {
    // wait
    std::this_thread::yield();
  }
}
}  // namespace sync
}  // namespace argcv
