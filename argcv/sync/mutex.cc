// Copyright 2021 Yu Jing
#include "argcv/sync/mutex.h"

#include "argcv/base/concurrency.h"

namespace argcv {
namespace sync {
//
void Mutex::Lock() noexcept {
  while (locked_.test_and_set(std::memory_order_acquire)) {
    Yield();
  }
}

}  // namespace sync
}  // namespace argcv
