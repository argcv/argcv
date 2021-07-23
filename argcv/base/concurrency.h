// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_CONCURRENCY_H_
#define ARGCV_BASE_CONCURRENCY_H_

#include "argcv/base/types.h"

namespace argcv {

/// Yield() basically means that the thread is not doing anything particularly
/// important and if any other threads or processes need to be run, they should
/// run. Otherwise, the current thread will continue to run.
void Yield() noexcept;

/// sleep(): This method causes the currently executing thread to sleep for the
/// specified number of milliseconds, subject to the precision and accuracy of
/// system timers and schedulers.
bool Sleep(uint64 millis, uint64 nanos = 0) noexcept;

/// \brief Returns the number of nano-seconds since the Unix epoch.
uint64 GetCurrentNanoseconds() noexcept;

/// \brief Returns the number of micro-seconds since the Unix epoch.
inline uint64 GetCurrentMicros() noexcept {
  return GetCurrentNanoseconds() / 1000L;
}

/// \brief Returns the number of milli-seconds since the Unix epoch.
inline uint64 GetCurrentMillis() noexcept {
  return GetCurrentNanoseconds() / 1000000L;
}

/// \brief Returns the number of milli-seconds since the Unix epoch.
inline uint64 GetCurrentMs() noexcept { return GetCurrentMillis(); }

/// \brief Returns the number of seconds since the Unix epoch.
inline uint64 GetCurrentSeconds() noexcept {
  return GetCurrentNanoseconds() / 1000000000L;
}

size_t GetThreadId() noexcept;

}  // namespace argcv

#endif  // ARGCV_BASE_CONCURRENCY_H_
