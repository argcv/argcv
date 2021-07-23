// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_ITER_H_
#define ARGCV_BASE_ITER_H_

#include <memory>

#include "argcv/base/macros.h"
#include "argcv/base/types.h"

namespace argcv {

/// \brief a iterable element
template <typename T>
class Iterator {
 public:
  virtual ~Iterator() {}
  virtual T* Next() const = 0;
};

}  // namespace argcv

#endif  // ARGCV_BASE_ITER_H_
