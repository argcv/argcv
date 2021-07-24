// Copyright 2021 Yu Jing
#include "argcv/base/chan.h"

#include "argcv/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using namespace argcv;

TEST(Channel, Base) {
  InitGoogleTestLogging();
  //
  Chan<int> ch(10);
  LOG(INFO) << "inited";
  for (int i = 0; i < 10; i++) {
    LOG(INFO) << "send: " << i;
    ch.Send(i);
  }
  ch.Close();  // means close input only

  LOG(INFO) << "all pushed";
  for (auto o : ch.Iter()) {
    LOG(INFO) << "out:" << o;
  }

  LOG(INFO) << "again";
  for (auto o : ch.Iter()) {
    LOG(INFO) << "out:" << o;
  }
  LOG(INFO) << "finished";
}
