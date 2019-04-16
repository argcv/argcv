// Copyright 2019 Yu Jing
#include "argcv/cxx/base/signal_slot.h"

#include <string>

#include "argcv/cxx/test/test.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::SignalSlot;

TEST(SignalSlot, SignalSlot) {
  InitGoogleTestLogging();

  SignalSlot<const std::string, const std::string> ss;
  //
  auto id1 = ss.Connect([](const std::string &k, const std::string &v) {
    LOG(INFO) << "id1: k:" << k << "v:" << v;
  });
  LOG(INFO) << "got id1:" << id1;

  auto id2 = ss.Connect([](const std::string &k, const std::string &v) {
    LOG(INFO) << "id2: k:" << k << "v:" << v;
  });
  LOG(INFO) << "got id2:" << id2;

  auto id3 = ss.Connect([](const std::string &k, const std::string &v) {
    LOG(INFO) << "id3: k:" << k << "v:" << v;
  });
  LOG(INFO) << "got id3:" << id3;

  ss.Disconnect(id2);

  ss.Call("aaa", "bbb");
}
