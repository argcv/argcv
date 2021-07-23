// Copyright 2021 Yu Jing
#include "argcv/base/concurrency.h"

#include <ctime>
#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield() and get_id()

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif  // __MACH__

namespace argcv {
void Yield() noexcept {
  /// ref: https://en.cppreference.com/w/cpp/thread/yield
  std::this_thread::yield();
}

bool Sleep(uint64 millis, uint64 nanos) noexcept {
  /// ref:
  /// http://pubs.opengroup.org/onlinepubs/9699919799/functions/nanosleep.html
  /// http://man7.org/linux/man-pages/man2/nanosleep.2.html
  uint64 secs = 0;
  nanos += millis * 1000000L;
  if (nanos > 1000000000L) {
    secs = nanos / 1000000000L;
    nanos = nanos % 1000000000L;
  }
  struct timespec req, rem;
  req.tv_sec = static_cast<time_t>(secs);
  req.tv_nsec = static_cast<long>(nanos);  // NOLINT(runtime/int)
  return nanosleep(&req, &rem) == 0;
}

uint64 GetCurrentNanoseconds() noexcept {
  struct timespec ts;
#ifdef __MACH__
  // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts.tv_sec = mts.tv_sec;
  ts.tv_nsec = mts.tv_nsec;
#else   // __MACH__
  clock_gettime(CLOCK_REALTIME, &ts);
#endif  // __MACH__
  return static_cast<uint64>(ts.tv_sec) * 1000000000L + ts.tv_nsec;
}

size_t GetThreadId() noexcept {
  // http://en.cppreference.com/w/cpp/thread/thread/id/hash
  std::hash<std::thread::id> hasher;
  return hasher(std::this_thread::get_id());
}

}  // namespace argcv
