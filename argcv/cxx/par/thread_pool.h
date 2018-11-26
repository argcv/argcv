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
#ifndef ARGCV_CXX_PAR_THREAD_POOL_H_
#define ARGCV_CXX_PAR_THREAD_POOL_H_

#include <condition_variable>  // NOLINT(build/c++11)
#include <functional>
#include <future>  // NOLINT(build/c++11)
#include <memory>
#include <mutex>  // NOLINT(build/c++11)
#include <queue>
#include <thread>  // NOLINT(build/c++11)
#include <utility>

// #include "glog/logging.h"

namespace argcv {

class ThreadPool {
 public:
  explicit ThreadPool(int reserve_threads, int max_parallelism = 0) noexcept
      : shutdown_(false),
        nthreads_(0),
        reserve_threads_(reserve_threads),
        max_parallelism_(max_parallelism),
        threads_waiting_(0) {
    for (int i = 0; i < reserve_threads_; i++) {
      std::lock_guard<std::mutex> lock(mu_);
      nthreads_++;
      new DynamicThread(this);
    }
  }

  ~ThreadPool() {
    std::unique_lock<std::mutex> lock(mu_);
    shutdown_ = true;
    cv_.notify_all();
    while (nthreads_ != 0) {
      shutdown_cv_.wait(lock);
    }
    CleanZombieThreads(&zombie_threads_);
  }

  // Send a callback function, expect nothing to return
  void Send(const std::function<void()>& task) noexcept {
    std::lock_guard<std::mutex> lock(mu_);
    // Add works to the callbacks list
    tasks_.emplace(task);
    // Increase pool size or notify as needed
    if (threads_waiting_ == 0 &&
        ((max_parallelism_ == 0 || (nthreads_ < max_parallelism_)))) {
      nthreads_++;
      new DynamicThread(this);
    } else {
      cv_.notify_one();
    }
    // Also use this chance to harvest dead threads
    if (!zombie_threads_.empty()) {
      CleanZombieThreads(&zombie_threads_);
    }
  }

  // add new work item to the pool
  template <class F, class... Args>
  auto Enqueue(F&& f, Args&&... args) noexcept  // NOLINT(build/c++11)
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f),
                  std::forward<Args>(args)...));  // NOLINT(build/c++11)

    std::future<return_type> res = task->get_future();

    Send([task]() { (*task)(); });
    return res;
  }

 private:
  class DynamicThread {
   public:
    explicit DynamicThread(ThreadPool* pool) noexcept
        : pool_(pool),
          thd_(new std::thread(&ThreadPool::DynamicThread::ThreadFunc, this)) {
      // LOG(INFO) << "ThreadPool::DynamicThread::DynamicThread"
      //           << pool_->nthreads_ << "." << pool_->threads_waiting_ << "."
      //           << pool_->zombie_threads_.size();
    }
    ~DynamicThread() {
      // LOG(INFO) << "ThreadPool::DynamicThread::~DynamicThread"
      //           << pool_->nthreads_ << "." << pool_->threads_waiting_ << "."
      //           << pool_->zombie_threads_.size();
      thd_->join();
      thd_.reset();
    }

   private:
    ThreadPool* pool_;
    std::unique_ptr<std::thread> thd_;
    void ThreadFunc() noexcept {
      pool_->ThreadFunc();
      // try kill self
      std::unique_lock<std::mutex> lock(pool_->mu_);
      pool_->nthreads_--;
      // move to zombie list
      pool_->zombie_threads_.emplace(this);

      if ((pool_->shutdown_) && (pool_->nthreads_ == 0)) {
        pool_->shutdown_cv_.notify_one();
      }
    }
  };
  bool shutdown_;
  int nthreads_;
  int reserve_threads_;
  int max_parallelism_;
  int threads_waiting_;
  std::condition_variable cv_;
  std::condition_variable shutdown_cv_;
  std::mutex mu_;
  std::queue<std::function<void()>> tasks_;
  std::queue<DynamicThread*> zombie_threads_;

  void ThreadFunc() noexcept {
    for (;;) {
      // Wait until work is available or we are shutting down.
      std::unique_lock<std::mutex> lock(mu_);
      if (!shutdown_ && tasks_.empty()) {
        // If there are too many threads waiting, then quit this thread
        if (threads_waiting_ >= reserve_threads_) {
          break;
        }
        threads_waiting_++;
        cv_.wait(lock);
        threads_waiting_--;
      }
      // Drain callbacks before considering shutdown to ensure all work
      // gets completed.
      if (!tasks_.empty()) {
        auto task = tasks_.front();
        tasks_.pop();
        lock.unlock();
        task();
      } else if (shutdown_) {
        break;
      }
    }
  }

  static void CleanZombieThreads(
      std::queue<DynamicThread*>* zombie_threads) noexcept {
    while (!zombie_threads->empty()) {
      DynamicThread* zombie_thread = zombie_threads->front();
      delete zombie_thread;
      zombie_threads->pop();
    }
  }
};

}  // namespace argcv

#endif  // ARGCV_CXX_PAR_THREAD_POOL_H_
