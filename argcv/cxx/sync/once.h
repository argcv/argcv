// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_SYNC_ONCE_H_
#define ARGCV_CXX_SYNC_ONCE_H_

#include <mutex>  // NOLINT(build/c++11)  std::once_flag

#include "argcv/cxx/base/macros.h"

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

#endif  // ARGCV_CXX_SYNC_ONCE_H_
