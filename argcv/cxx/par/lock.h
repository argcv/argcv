/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Yu Jing
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#ifndef ARGCV_CXX_PAR_LOCK_H_
#define ARGCV_CXX_PAR_LOCK_H_

#include <atomic>
#include <memory>
#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

#include "argcv/cxx/par/atomic.h"

namespace argcv {

class LockGuard;
class SoftLock {
 public:
  virtual void Lock() noexcept = 0;
  virtual void Unlock() noexcept = 0;
  LockGuard Guard() noexcept;
};

class LockGuard {
 public:
  explicit LockGuard(SoftLock* lock) : lock_(lock) { lock_->Lock(); }
  ~LockGuard() { lock_->Unlock(); }

 private:
  SoftLock* lock_;
};

class Spinlock : public SoftLock {
 public:
  // ATOMIC_FLAG_INIT = {false}
  Spinlock() noexcept : locked_(std::make_shared<std::atomic_flag>()) {}
  Spinlock(const Spinlock& lock) noexcept : locked_(lock.locked_) {}
  void Lock() noexcept {
    while (locked_->test_and_set(std::memory_order_acquire)) {
      // wait
      std::this_thread::yield();
    }
  }
  bool TryLock() noexcept {
    return locked_->test_and_set(std::memory_order_acquire);
  }
  void Unlock() noexcept { locked_->clear(std::memory_order_release); }

 private:
  std::shared_ptr<std::atomic_flag> locked_;
};

class TicketLock : public SoftLock {
 public:
  TicketLock() noexcept : ticket_(0), serve_(0) {}
  TicketLock(const TicketLock& lock) noexcept
      : ticket_(lock.ticket_), serve_(lock.serve_) {}
  void Lock() noexcept {
    auto t = ticket_.FetchAdd(1);
    while (t != serve_.underlying_ref()) {
      // wait
      std::this_thread::yield();
    }
  }
  void Unlock() noexcept { serve_.FetchAdd(1); }

 private:
  SharedAtomic<uint32_t> ticket_;
  SharedAtomic<uint32_t> serve_;
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_LOCK_H_
