// Copyright 2019 Yu Jing
#include "argcv/cxx/sync/base.h"

namespace argcv {
namespace sync {

LockGuard Locker::Guard() noexcept {
  // defined in base.h
  return LockGuard(this);
}

}  // namespace sync

}  // namespace argcv
