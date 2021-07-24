// Copyright 2021 Yu Jing
#ifndef ARGCV_BASE_CHAN_H_
#define ARGCV_BASE_CHAN_H_

#include <condition_variable>
#include <mutex>
#include <queue>

#include "argcv/base/iter.h"
#include "argcv/base/macros.h"

namespace argcv {

/**
 * @brief Chan
 *
 * Comes from facebook's work, may also refer to
 * https://github.com/facebook/folly/blob/master/folly/ProducerConsumerQueue.h
 *
 *
 * @tparam T
 */
template <typename T>
class Chan : public Iterable<T> {
 public:
  /**
   * @brief Construct a new Chan object
   *
   * @param limit a non-negative data, 0 for unlimited
   */
  explicit Chan(int limit = 0) : limit_(limit), closed_(false) {}

  virtual ~Chan() {
    // auto close
    Close();
  }

  /**
   * @brief Iter
   *
   * get a iterator for the input
   *
   * @return Iterator<T>
   */
  Iterator<T> Iter() noexcept { return Iterator<T>(this); }

  /**
   * @brief Send a
   *
   * @param elem
   */
  void Send(T elem) {
    std::unique_lock<std::mutex> lock(mutex_);
    input_cv_.wait(lock, [this]() { return !isFull() || closed_; });

    if (closed_) {
      return;
    }

    queue_.push(std::move(elem));

    if (!isFull()) {
      input_cv_.notify_one();
    }
    output_cv_.notify_one();
  }

  /**
   * @brief Next
   *
   * @param elem
   * @return true got a new value
   * @return false end of input (and will always to be false in the future, to
   * be sure)
   */
  bool Next(T &elem) noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    output_cv_.wait(lock, [this]() { return !isEmpty() || closed_; });
    if (isEmpty()) {
      return false;
    }
    elem = std::move(queue_.front());
    queue_.pop();

    if (!isEmpty()) {
      output_cv_.notify_one();
    }
    input_cv_.notify_one();

    return true;
  }

  /**
   * @brief Close
   *
   * Close input
   *
   * - mark closed_ as true
   * - notify output_cv_ to unlock "Next"
   */
  void Close() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    closed_ = true;
    output_cv_.notify_all();
  }

  /**
   * @brief Clear resets the queue
   *
   * - Clears the queue.
   * - Resets the status of the queue to be adding-unfinished.
   * - Notifies all the consumers and producers to work.
   */
  void Clear() noexcept {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!isEmpty()) {
      queue_.pop();
    }
    closed_ = false;

    input_cv_.notify_all();
    output_cv_.notify_all();
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Chan);

  size_t limit_;
  bool closed_;

  std::condition_variable input_cv_;
  std::condition_variable output_cv_;

  std::mutex mutex_;
  std::queue<T> queue_;

  bool isFull() const noexcept { return limit_ > 0 && queue_.size() >= limit_; }

  bool isEmpty() const noexcept { return queue_.size() == 0; }
};

}  // namespace argcv

#endif  // ARGCV_BASE_CHAN_H_
