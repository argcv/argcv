// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_ITER_H_
#define ARGCV_CXX_BASE_ITER_H_

#include <memory>

#include "argcv/cxx/base/macros.h"
#include "argcv/cxx/base/types.h"

namespace argcv {

/// \brief a iterable element
template <typename T>
class Iterator {
 public:
  virtual ~Iterator() {}
  virtual T* Next() const = 0;
};

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_ITER_H_
