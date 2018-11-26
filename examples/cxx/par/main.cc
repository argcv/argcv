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

#include "argcv/cxx/par/atomic.h"
#include "argcv/cxx/par/thread_pool.h"
#include "glog/logging.h"

using argcv::AtomicFetchAdd;
using argcv::ThreadPool;

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);

  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  ThreadPool pool(3);

  int out = 0;
  for (int i = 0; i < 10; i++) {
    pool.Send([&out, i] {
      LOG(INFO) << "[hello] -- start..." << i << "#" << out;
      out += i;
      usleep(500 * 1000);
      LOG(INFO) << "[hello] -- end..." << i << "#" << out;
    });
  }

  size_t task_size = 100;
  std::atomic<uint32_t> apple(0);
  std::vector<std::future<uint32_t> > results;
  uint32_t gt_score = 0;
  for (size_t i = 0; i < task_size; i++) {
    gt_score += (uint32_t)i;
    results.emplace_back(pool.Enqueue([i, task_size, &apple] {
      LOG(INFO) << "[test_case_threads] start: " << i << " of [" << task_size
                << "]";
      // usleep(i);
      usleep(500 * 1000);
      uint32_t res = AtomicFetchAdd<uint32_t>(&apple, (uint32_t)1);
      LOG(INFO) << "[test_case_threads] finished: " << i << " of [" << task_size
                << "]";
      return res;
    }));
  }

  uint32_t score = 0;
  for (auto&& result : results) {
    score += static_cast<uint32_t>(result.get());
  }

  LOG(INFO) << "gt_score:" << gt_score << ", future_score:" << score;

  return 0;
}
