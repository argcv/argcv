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
#ifndef ARGCV_CXX_PAR_ATOMIC_H_
#define ARGCV_CXX_PAR_ATOMIC_H_

#include <atomic>
#include <memory>
#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

namespace argcv {

// http://en.cppreference.com/w/cpp/language/type_alias
template <typename T>
using atomic = std::atomic<T>;  // typedef std::atomic<T> as atomic<T>

template <typename T>
T AtomicFetchAdd(atomic<T>* obj, T arg) noexcept {
  return obj->fetch_add(arg);
}

template <typename T>
T AtomicFetchSub(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_sub(arg);
}

template <typename T>
T AtomicFetchOr(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_or(arg);
}

template <typename T>
T AtomicFetchXor(atomic<T>* obj, const T& arg) noexcept {
  return obj->fetch_xor(arg);
}

template <typename T>
void AtomicStore(atomic<T>* obj, const T& arg) noexcept {
  return obj->store(arg);
}

template <typename T>
T AtomicLoad(atomic<T>* obj) noexcept {
  return obj->load();
}

template <typename T>
class SharedAtomic {
 public:
  SharedAtomic() noexcept : data_(std::make_shared<atomic<T>>()) {}
  explicit SharedAtomic(const T& desr) noexcept
      : data_(std::make_shared<atomic<T>>()) {
    Store(desr);
  }
  SharedAtomic(const SharedAtomic<T>& data) noexcept : data_(data.data_) {}

  T FetchAdd(const T& arg) noexcept {
    return AtomicFetchAdd<T>(data_.get(), arg);
  }

  T SyncAdd(const T& arg) noexcept {
    return AtomicFetchAdd<T>(data_.get(), arg) + arg;
  }

  T FetchSub(const T& arg) noexcept {
    return AtomicFetchSub<T>(data_.get(), arg);
  }

  T SyncSub(const T& arg) noexcept {
    return AtomicFetchSub<T>(data_.get(), arg) - arg;
  }

  T FetchOr(const T& arg) noexcept {
    return AtomicFetchOr<T>(data_.get(), arg);
  }

  T FetchXor(const T& arg) noexcept {
    return AtomicFetchXor<T>(data_.get(), arg);
  }

  T Load() noexcept { return AtomicLoad<T>(data_.get()); }

  void Store(const T& desr) noexcept {
    return AtomicStore<T>(data_.get(), desr);
  }

  atomic<T>& underlying_ref() noexcept { return *data_.get(); }

 private:
  std::shared_ptr<atomic<T>> data_;
};

}  // namespace argcv

#endif  //  ARGCV_CXX_PAR_ATOMIC_H_
