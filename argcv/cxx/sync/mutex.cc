// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/mutex.h"

#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

namespace argcv {
namespace sync {
//
void Mutex::Lock() noexcept {
  while (locked_.test_and_set(std::memory_order_acquire)) {
    // wait
    std::this_thread::yield();
  }
}

}  // namespace sync
}  // namespace argcv
