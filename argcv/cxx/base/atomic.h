// Copyright 2019 Yu Jing
#ifndef ARGCV_CXX_BASE_ATOMIC_H_
#define ARGCV_CXX_BASE_ATOMIC_H_

#include <atomic>
#include <memory>

namespace argcv {

/**
 *  \brief fetch_add
 */
template <typename T>
T AtomicFetchAdd(std::atomic<T>* obj, T arg) noexcept {
  return obj->fetch_add(arg);
}

/**
 *  \brief fetch_sub
 */
template <typename T>
T AtomicFetchSub(std::atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_sub(arg);
}

/**
 *  \brief fetch_or
 */
template <typename T>
T AtomicFetchOr(std::atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_or(arg);
}

/**
 *  \brief fetch_xor
 */
template <typename T>
T AtomicFetchXor(std::atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_xor(arg);
}

/**
 *  \brief store
 */
template <typename T>
void AtomicStore(std::atomic<T>* obj, const T& arg) noexcept {
  return obj->store(arg);
}

/**
 *  \brief load
 */
template <typename T>
T AtomicLoad(std::atomic<T>* obj) noexcept {
  return obj->load();
}

}  // namespace argcv

#endif  //  ARGCV_CXX_BASE_ATOMIC_H_
