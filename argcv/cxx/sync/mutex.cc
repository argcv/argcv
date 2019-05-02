// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/mutex.h"

#include "argcv/cxx/base/concurrency.h"

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
