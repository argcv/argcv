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
#include <cstdio>

#include <unistd.h>

#include <functional>
#include <thread>  // NOLINT(build/c++11) for std::this_thread::yield();

#include "argcv/cxx/par/atomic.h"
#include "argcv/cxx/par/duration.h"
#include "argcv/cxx/par/thread_pool.h"
#include "concurrentqueue.h"
#include "glog/logging.h"

using argcv::AtomicFetchAdd;
using argcv::Duration;
using argcv::ThreadPool;
using moodycamel::ConcurrentQueue;

// ref: https://github.com/cameron314/concurrentqueue/blob/master/samples.md
int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  ConcurrentQueue<int> q;

  const int num_producer = 100;
  const int num_consumer = 20;

  Duration wait = Duration::NewMilliseconds(20);

  std::atomic<int> num_finished_producer(0);
  // std::atomic<int> num_finished_consumer(0);

  std::atomic<int> total(0);

  {
    ThreadPool pool(3, 120);

    for (int i = 0; i != num_producer; i++) {
      pool.Send([&q, &num_finished_producer, i, wait] {
        wait.Wait();
        q.enqueue(i * 2);
        q.enqueue(i * 2 + 1);
        int curr_finished_producer =
            num_finished_producer.fetch_add(2, std::memory_order_release);
        LOG(INFO) << "sent.." << i << " in total: " << curr_finished_producer;
      });
    }

    for (int i = 0; i != num_consumer; i++) {
      pool.Send([&q, &num_finished_producer, &total, num_producer, i] {
        bool producer_is_done = false;
        int val_recv;
        while (true) {
          producer_is_done =
              num_finished_producer.load(std::memory_order_acquire) ==
              (num_producer * 2);
          if (q.try_dequeue(val_recv)) {
            total.fetch_add(val_recv, std::memory_order_release);
            LOG(INFO) << "[" << i << "] recv.." << val_recv
                      << " in total: " << total.load(std::memory_order_acquire);
          } else if (producer_is_done) {
            LOG(INFO) << "[" << i << "] out";
            break;
          } else {
            LOG(INFO) << "[" << i << "] wait...";
            std::this_thread::yield();
          }
        }
      });
    }
  }

  LOG(INFO) << "all done, total:" << total.load(std::memory_order_acquire);

  return 0;
}
