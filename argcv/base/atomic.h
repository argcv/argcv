// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_ATOMIC_H_
#define ARGCV_BASE_ATOMIC_H_

#include <atomic>
#include <memory>

#include "argcv/base/types.h"

namespace argcv {

/**
 *  \brief fetch_add
 */
template <typename T>
T AtomicFetchAdd(atomic<T>* obj, T arg) noexcept {
  return obj->fetch_add(arg);
}

/// increment 1
template <typename T>
T AtomicIncr(atomic<T>* obj) noexcept {
  return AtomicFetchAdd<T>(obj, static_cast<T>(1));
}

/**
 *  \brief fetch_sub
 */
template <typename T>
T AtomicFetchSub(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_sub(arg);
}

// decrement
template <typename T>
T AtomicDecr(atomic<T>* obj) noexcept {
  return AtomicFetchSub<T>(obj, static_cast<T>(1));
}

/**
 *  \brief fetch_or
 */
template <typename T>
T AtomicFetchOr(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_or(arg);
}

/**
 *  \brief fetch_xor
 */
template <typename T>
T AtomicFetchXor(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_xor(arg);
}

/**
 *  \brief store
 */
template <typename T>
void AtomicStore(atomic<T>* obj, const T& arg) noexcept {
  return obj->store(arg);
}

/**
 *  \brief load
 */
template <typename T>
T AtomicLoad(const atomic<T>* obj) noexcept {
  return obj->load();
}

}  // namespace argcv

#endif  //  ARGCV_BASE_ATOMIC_H_
