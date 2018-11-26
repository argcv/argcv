/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================
Original Source: https://goo.gl/EtF4JS
  aka: tensorflow/core/lib/strings/scanner.h@tensorflow

Revised By: Yu Jing
*/
#include "argcv/cxx/str/str_scan.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <initializer_list>
#include <string>
#include <vector>

#include "argcv/c/char/char_helper.h"
#include "argcv/cxx/str/str_slice.h"

namespace argcv {

bool StrScan::GetResult(StrSlice *remaining, StrSlice *capture) noexcept {
  if (error_) {
    return false;
  }
  if (remaining != nullptr) {
    remaining->Reset();
    remaining->Append(cur_.ToString());
  }
  if (capture != nullptr) {
    capture->Reset();
    capture->Append(capture_.substr(0, pos_moved_));
  }
  return true;
}

void StrScan::ScanUntilImpl(char end_ch, bool escaped) noexcept {
  for (;;) {
    if (cur_.Empty()) {
      Error();
      return;
    }
    const char ch = cur_[0];
    if (ch == end_ch) {
      return;
    }

    Drop(1);
    if (escaped && ch == '\\') {
      // Escape character, skip next character.
      if (cur_.Empty()) {
        Error();
        return;
      }
      Drop(1);
    }
  }
}

}  // namespace argcv
