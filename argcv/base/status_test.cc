// Copyright 2021 Yu Jing
#include "argcv/base/status.h"

#include "argcv/test/test.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

using argcv::Status;

TEST(Status, ToString) {
  // init log
  InitGoogleTestLogging();
  Status st_ok = Status::OK();
  LOG(INFO) << "OK =>" << st_ok.ok() << ", str:" << st_ok.ToString();
  CHECK_OK(st_ok);

  st_ok.AppendToMessage("message");

  LOG(INFO) << "OK? => " << st_ok.ok() << ", str:" << st_ok.ToString()
            << "\n error message [" << st_ok.error_message() << "]";

  Status st_canceled = Status::Cancelled("my cancel", " reason", 1, 2, 3);

  LOG(INFO) << "OK =>" << st_canceled.ok() << ", str:" << st_canceled.ToString()
            << "\nError message [" << st_canceled.error_message() << "]"
            << "\nCheck is_canceled:" << st_canceled.IsCancelled();

  st_canceled.AppendToMessage("message2");

  LOG(INFO) << "OK =>" << st_canceled.ok() << ", str:" << st_canceled.ToString()
            << "\nError message [" << st_canceled.error_message() << "]"
            << "\nCheck is_canceled:" << st_canceled.IsCancelled();
}
