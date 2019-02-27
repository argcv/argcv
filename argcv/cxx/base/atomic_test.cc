// Copyright 2019 Yu Jing
#include "argcv/cxx/base/atomic.h"

#include <atomic>
#include <thread>  // NOLINT build/c++11

#include "argcv/cxx/base/test.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {

void do_work(atomic<int> *data) {
  data->fetch_add(1, std::memory_order_relaxed);
}

TEST(Atomic, Atomic) {
  InitTestGoogleLogging();
  atomic<int> data(0);

  std::thread th1(do_work, &data);
  std::thread th2(do_work, &data);
  std::thread th3(do_work, &data);
  std::thread th4(do_work, &data);
  std::thread th5(do_work, &data);

  th1.join();
  th2.join();
  th3.join();
  th4.join();
  th5.join();
  LOG(INFO) << "Result:" << data;
  EXPECT_EQ(data, 5);
}
}  // namespace argcv
