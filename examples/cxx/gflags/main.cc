// Copyright 2019 Yu Jing

//! this must go before the #include!
#define STRIP_FLAG_HELP \
  1  //!< see: https://gflags.github.io/gflags/#misc <br/>
     //!< by the way, for doc blocks: http://bit.ly/2UjnmBT
#include <iostream>
#include <string>
#include <vector>

#include "gflags/gflags.h"
#include "gflags/gflags_completions.h"
#include "glog/logging.h"

// using namespace std;
using std::string;

DEFINE_bool(big_menu, true, "Include 'advanced' options in the menu listing");
DEFINE_string(languages, "english,french,german",
              "comma-separated list of languages to offer in the 'lang' menu");
DEFINE_bool(consider_made_up_languages, false, "Consider Made Up Languages");
DEFINE_bool(h, false, "Show help");

/**
 * \brief A simple example to pass parameters into the code. <br/>
 *        e.g. `bazel run //examples/cxx/gflags:hello_gflags --nobig-menu
 * --languages="english,french,german,chinese"`
 *
 * + `DEFINE_int32`: 32-bit integer
 * + `DEFINE_int64`: 64-bit integer
 * + `DEFINE_uint32`: unsigned 32-bit integer
 * + `DEFINE_uint64`: unsigned 64-bit integer
 * + `DEFINE_double`: double
 * + `DEFINE_string`: C++ string
 *
 * options examples:
 *
 *  + --nobig-menu
 *  + --big-menu=false
 *  + --languages="english,french,german,chinese"
 */
int main(int argc, char* argv[]) {
  // glogs
  google::InitGoogleLogging(argv[0]);
  FLAGS_log_dir = ".";
  FLAGS_stderrthreshold = 0;  // 2 in default
  FLAGS_minloglevel = 0;
  FLAGS_colorlogtostderr = true;

  gflags::RegisterFlagValidator(
      &FLAGS_big_menu, [](const char* flagname, bool value) -> bool {
        LOG(INFO) << "big_menu is " << (value ? "TRUE" : "FALSE");
        return true;
      });

  for (int i = 0; i < argc; i++) {
    LOG(INFO) << "argv[" << i << "] = " << argv[i];
  }
  LOG(INFO) << "----- end -----";

  gflags::SetVersionString("0.0.1");
  gflags::SetUsageMessage("Usage : ./hello_gflags ");
  // gflags::ShowUsageWithFlagsRestrict(argv[0], "hello_gflags.cc");
  // gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
  gflags::HandleCommandLineHelpFlags();
  if (FLAGS_consider_made_up_languages)
    FLAGS_languages += ",klingon";  // implied by --consider_made_up_languages
  if (FLAGS_languages.find("finnish") != string::npos) {
    LOG(INFO) << "find: finnish";
  }

  LOG(INFO) << "[flag:] big_menu " << FLAGS_big_menu;
  LOG(INFO) << "[flag:] languages " << FLAGS_languages;
  LOG(INFO) << "[flag:] consider_made_up_languages "
            << FLAGS_consider_made_up_languages;

  for (int i = 0; i < argc; i++) {
    LOG(INFO) << "argv[" << i << "] = " << argv[i];
  }
  LOG(INFO) << "----- end -----";

  LOG(INFO) << "finsih all";
  gflags::ShutDownCommandLineFlags();
}
