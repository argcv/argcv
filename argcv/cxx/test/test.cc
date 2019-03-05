// Copyright 2019 Yu Jing
#include "argcv/cxx/test/test.h"

#include <unistd.h>  // usleep microseconds
#include <mutex>     // NOLINT(build/c++11)  std::once_flag

#include "glog/logging.h"
#include "gtest/gtest.h"

// TEST(AssertUseCS, WaitTest) { ASSERT_USECS(200, usleep(10)); }

static std::once_flag glog_init;

void InitGoogleTestLogging() {
  std::call_once(glog_init, []() {
    google::InitGoogleLogging("argcv-testing");

    // log_dir (string, default="")
    // If specified, logfiles are written into this directory instead of the
    // default logging directory.
    FLAGS_log_dir = "";

    // logtostderr (bool, default=false)
    // Log messages to stderr instead of logfiles.
    // Note:
    // you can set binary flags to true by specifying 1, true, or yes (case
    // insensitive). Also, you can set binary flags to false by specifying 0,
    // false, or no (again, case insensitive).
    FLAGS_logtostderr = true;

    // stderrthreshold (int, default=2, which is ERROR)
    // Copy log messages at or above this level to stderr in addition to
    // logfiles. The numbers of severity levels INFO, WARNING, ERROR, and FATAL
    // are 0, 1, 2, and 3, respectively.
    FLAGS_stderrthreshold = 2;

    // minloglevel (int, default=0, which is INFO)
    // Log messages at or above this level. Again, the numbers of severity
    // levels INFO, WARNING, ERROR, and FATAL are 0, 1, 2, and 3, respectively.
    FLAGS_minloglevel = 0;

    // Set color messages logged to stderr (if supported by terminal).
    FLAGS_colorlogtostderr = true;

    FLAGS_log_prefix = true;

    FLAGS_stop_logging_if_full_disk = true;
  });
}
