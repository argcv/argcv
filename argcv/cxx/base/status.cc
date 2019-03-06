// Copyright 2019 Yu Jing
#include "argcv/cxx/base/status.h"

#include <ostream>
#include <string>

namespace argcv {

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

}  // namespace argcv
