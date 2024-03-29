// Copyright 2021 Yu Jing
#include "argcv/path/path.h"

#include <string.h>  // memcpy(void *restrict dst, const void *restrict src, size_t n);

#include <string>

#include "absl/strings/str_join.h"
#include "argcv/base/types.h"

namespace argcv {

namespace path {
namespace internal {
PathBuff::PathBuff(const string_view &data, size_t len) noexcept
    : sz_(len), sz_buff_(2 * len + 1) {
  buff_ = new char[sz_buff_];
  if (sz_ > 0) {
    memcpy(buff_, data.data(), sz_);
  }
}

PathBuff &PathBuff::Append(char c) noexcept {
  if (sz_ >= sz_buff_) {
    sz_buff_ = sz_buff_ * 2 + 1;
    char *new_buff = new char[sz_buff_];
    memcpy(new_buff, buff_, sz_);
    delete[] buff_;
    buff_ = new_buff;
  }
  buff_[sz_] = c;
  sz_++;
  return *this;
}

size_t PathBuff::Backspace() noexcept {
  if (sz_ > 0) {
    sz_--;
  }
  return sz_;
}

string JoinPathImpl(std::initializer_list<const string_view> paths) noexcept {
  return CleanPath(absl::StrJoin(paths, "/"));
}

}  // namespace internal
}  // namespace path

}  // namespace argcv
