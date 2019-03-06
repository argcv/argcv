// Copyright 2019 Yu Jing
#include "argcv/cxx/base/status.h"

#include <ostream>

namespace argcv {

const std::string& Status::empty_string() {
  static std::string empty;
  return empty;
}

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

}  // namespace argcv
