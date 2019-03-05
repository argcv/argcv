// Copyright 2019 Yu Jing
// Please see https://github.com/abseil/abseil-cpp/tree/master/absl for detail
#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"
#include "glog/logging.h"

/**
 * \brief A simple string concat example
 */
std::string HelloStrCat(absl::string_view name) {
  return absl::StrCat("Hello [", name, "]");
}

int main(int argc, char const *argv[]) {
  LOG(INFO) << HelloStrCat("foo~~");
  return 0;
}
