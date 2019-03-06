// Copyright 2019 Yu Jing
#include "argcv/cxx/base/result.h"

#include "argcv/cxx/base/status.h"
#include "argcv/cxx/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::Result;
using argcv::Status;

TEST(Result, Result) {
  // init log
  InitGoogleTestLogging();
  argcv::Result<size_t> r = Result<size_t>(argcv::Status::OK(), 1);

  LOG(INFO) << "ok:" << r.ok();
  EXPECT_TRUE(r.ok());
  LOG(INFO) << "status:" << r.status().ToString();
  EXPECT_EQ("OK", r.status().ToString());
  LOG(INFO) << "data:" << r.data();
  EXPECT_EQ(1, r.data());
}
