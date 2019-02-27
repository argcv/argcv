// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_ATOMIC_H_
#define ARGCV_CXX_BASE_ATOMIC_H_

#include <atomic>

namespace argcv {

// typedef std::atomic<T> as atomic<T>
template <typename T>
using atomic = std::atomic<T>;

}  // namespace argcv

#endif  // ARGCV_CXX_BASE_ATOMIC_H_
