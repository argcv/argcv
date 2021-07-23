// Copyright 2021 Yu Jing
#include "argcv/base/status.h"

#include <ostream>
#include <string>

namespace argcv {

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

}  // namespace argcv
