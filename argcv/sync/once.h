// Copyright 2021 Yu Jing
#ifndef ARGCV_SYNC_ONCE_H_
#define ARGCV_SYNC_ONCE_H_

#include <mutex>  // NOLINT(build/c++11)  std::once_flag

#include "argcv/base/macros.h"

namespace argcv {

namespace sync {

class Once {
 public:
  Once() {}

  virtual ~Once() {}

  template <class F, class... Args>
  void Do(F&& f, Args&&... args) noexcept {
    std::call_once(flag_, [&f, &args...]() { f(args...); });
  }

 private:
  std::once_flag flag_;

  DISALLOW_COPY_AND_ASSIGN(Once);
};

}  // namespace sync

}  // namespace argcv

#endif  // ARGCV_SYNC_ONCE_H_
