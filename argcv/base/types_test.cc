// Copyright 2021 Yu Jing
#include "argcv/base/types.h"

#include <atomic>
#include <thread>  // NOLINT build/c++11

#include "argcv/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

namespace argcv {

TEST(Types, Atomic) {
  auto incr = [](atomic<int>* data) {
    data->fetch_add(1, std::memory_order_relaxed);
  };

  InitGoogleTestLogging();
  atomic<int> data(0);

  std::thread th1(incr, &data);
  std::thread th2(incr, &data);
  std::thread th3(incr, &data);
  std::thread th4(incr, &data);
  std::thread th5(incr, &data);

  th1.join();
  th2.join();
  th3.join();
  th4.join();
  th5.join();
  LOG(INFO) << "Result:" << data;
  EXPECT_EQ(data, 5);
}

}  // namespace argcv
