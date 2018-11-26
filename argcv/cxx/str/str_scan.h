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
Original Source: https://goo.gl/95hRQa
  aka: tensorflow/core/lib/strings/scanner.h@tensorflow

Revised By: Yu Jing
*/
#ifndef ARGCV_CXX_STR_STR_SCAN_H_
#define ARGCV_CXX_STR_STR_SCAN_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <initializer_list>
#include <string>
#include <vector>

#include "argcv/cxx/env/macros.h"
#include "argcv/cxx/str/str_slice.h"

namespace argcv {

class StrScan {
 public:
  // Classes of characters. Each enum name is to be read as the union of the
  // parts - e.g., class LETTER_DIGIT means the class includes all letters and
  // all digits.
  //
  // LETTER means ascii letter a-zA-Z.
  // DIGIT means ascii digit: 0-9.
  enum CharClass {
    // NOTE: When adding a new CharClass, update the AllCharClasses ScannerTest
    // in scanner_test.cc
    ALL,
    DIGIT,
    LETTER,
    LETTER_DIGIT,
    LETTER_DIGIT_DASH_UNDERSCORE,
    LETTER_DIGIT_DASH_DOT_SLASH,             // SLASH is / only, not backslash
    LETTER_DIGIT_DASH_DOT_SLASH_UNDERSCORE,  // SLASH is / only, not backslash
    LETTER_DIGIT_DOT,
    LETTER_DIGIT_DOT_PLUS_MINUS,
    LETTER_DIGIT_DOT_UNDERSCORE,
    LETTER_DIGIT_UNDERSCORE,
    LOWERLETTER,
    LOWERLETTER_DIGIT,
    LOWERLETTER_DIGIT_UNDERSCORE,
    NON_ZERO_DIGIT,
    SPACE,
    UPPERLETTER,
  };

  explicit StrScan(const std::string &data) noexcept
      : capture_(data), pos_moved_(0), error_(false) {
    cur_ = StrSlice(data);
  }

  explicit StrScan(const StrSlice &data) noexcept
      : capture_(data.ToString()), pos_moved_(0), error_(false), cur_(data) {}

  // Consume the next character of the given class from input. If the next
  // character is not in the class, then GetResult will ultimately return false.
  StrScan &One(CharClass clz) noexcept {
    if (cur_.Empty() || !Matches(clz, cur_[0])) {
      return Error();
    }
    Drop(1);
    return *this;
  }

  // Consume the next s.size() characters of the input, if they match <s>. If
  // they don't match <s>, this is a no-op.
  StrScan &ZeroOrOneLiteral(const std::string &s) noexcept {
    Consume(s);
    return *this;
  }

  // Consume the next s.size() characters of the input, if they match <s>. If
  // they don't match <s>, this is a no-op.
  StrScan &ZeroOrOneLiteral(const StrSlice &s) noexcept {
    Consume(s);
    return *this;
  }

  // Consume the next s.size() characters of the input, if they match <s>. If
  // they don't match <s>, then GetResult will ultimately return false.
  StrScan &OneLiteral(const std::string &s) noexcept {
    if (!Consume(s)) {
      error_ = true;
    }
    return *this;
  }

  // Consume the next s.size() characters of the input, if they match <s>. If
  // they don't match <s>, then GetResult will ultimately return false.
  StrScan &OneLiteral(const StrSlice &s) noexcept {
    if (!Consume(s)) {
      error_ = true;
    }
    return *this;
  }

  // Consume characters from the input as long as they match <clz>. Zero
  // characters is still considered a match, so it will never cause GetResult to
  // return false.
  StrScan &Any(CharClass clz) noexcept {
    while (!cur_.Empty() && Matches(clz, cur_[0])) {
      Drop(1);
    }
    return *this;
  }

  // Shorthand for One(clz).Any(clz).
  StrScan &Many(CharClass clz) noexcept { return One(clz).Any(clz); }

  // Reset the capture start point.
  //
  // Later, when GetResult is called and if it returns true, the capture
  // returned will start at the position at the time this was called.
  StrScan &RestartCapture() noexcept {
    capture_ = cur_.ToString();
    pos_moved_ = 0;
    return *this;
  }

  // Stop capturing input.
  //
  // Later, when GetResult is called and if it returns true, the capture
  // returned will end at the position at the time this was called.
  // Maybe it is not require in current data structure
  StrScan &StopCapture() noexcept {
    capture_.erase(capture_.begin() + pos_moved_, capture_.end());
    return *this;
  }

  // If not at the input of input, then GetResult will ultimately return false.
  StrScan &Eos() noexcept {
    if (!cur_.Empty()) error_ = true;
    return *this;
  }

  // Shorthand for Any(SPACE).
  StrScan &AnySpace() noexcept { return Any(SPACE); }

  // This scans input until <end_ch> is reached. <end_ch> is NOT consumed.
  StrScan &ScanUntil(char end_ch) noexcept {
    ScanUntilImpl(end_ch, false);
    return *this;
  }

  // This scans input until <end_ch> is reached. <end_ch> is NOT consumed.
  // Backslash escape sequences are skipped.
  // Used for implementing quoted string scanning.
  StrScan &ScanEscapedUntil(char end_ch) noexcept {
    ScanUntilImpl(end_ch, true);
    return *this;
  }

  // Return the next character that will be scanned, or <default_value> if there
  // are no more characters to scan.
  // Note that if a scan operation has failed (so GetResult() returns false),
  // then the value of Peek may or may not have advanced since the scan
  // operation that failed.
  char Peek(char default_value = '\0') const noexcept {
    return cur_.Empty() ? default_value : cur_[0];
  }

  // Returns false if there are no remaining characters to consume.
  int Empty() const noexcept { return cur_.Empty(); }

  bool GetResult(StrSlice *remaining = nullptr,
                 StrSlice *capture = nullptr) noexcept;

  bool IsError() const noexcept { return error_; }

  StrScan &Error() noexcept {
    error_ = true;
    return *this;
  }

  static bool IsLetter(char ch) noexcept {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  static bool IsLowerLetter(char ch) noexcept { return ch >= 'a' && ch <= 'z'; }

  static bool IsDigit(char ch) noexcept { return ch >= '0' && ch <= '9'; }

  static bool IsSpace(char ch) noexcept {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' ||
            ch == '\r');
  }

  static bool Matches(CharClass clz, char ch) noexcept {
    switch (clz) {
      case ALL:
        return true;
      case DIGIT:
        return IsDigit(ch);
      case LETTER:
        return IsLetter(ch);
      case LETTER_DIGIT:
        return IsLetter(ch) || IsDigit(ch);
      case LETTER_DIGIT_DASH_UNDERSCORE:
        return (IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '_');
      case LETTER_DIGIT_DASH_DOT_SLASH:
        return IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '.' ||
               ch == '/';
      case LETTER_DIGIT_DASH_DOT_SLASH_UNDERSCORE:
        return (IsLetter(ch) || IsDigit(ch) || ch == '-' || ch == '.' ||
                ch == '/' || ch == '_');
      case LETTER_DIGIT_DOT:
        return IsLetter(ch) || IsDigit(ch) || ch == '.';
      case LETTER_DIGIT_DOT_PLUS_MINUS:
        return IsLetter(ch) || IsDigit(ch) || ch == '+' || ch == '-' ||
               ch == '.';
      case LETTER_DIGIT_DOT_UNDERSCORE:
        return IsLetter(ch) || IsDigit(ch) || ch == '.' || ch == '_';
      case LETTER_DIGIT_UNDERSCORE:
        return IsLetter(ch) || IsDigit(ch) || ch == '_';
      case LOWERLETTER:
        return ch >= 'a' && ch <= 'z';
      case LOWERLETTER_DIGIT:
        return IsLowerLetter(ch) || IsDigit(ch);
      case LOWERLETTER_DIGIT_UNDERSCORE:
        return IsLowerLetter(ch) || IsDigit(ch) || ch == '_';
      case NON_ZERO_DIGIT:
        return IsDigit(ch) && ch != '0';
      case SPACE:
        return IsSpace(ch);
      case UPPERLETTER:
        return ch >= 'A' && ch <= 'Z';
    }
    return false;
  }

 private:
  std::string capture_;
  size_t pos_moved_;
  bool error_;
  StrSlice cur_;

  // Checks whether const StrSlice & starts with x and if so advances
  // the beginning of it to past the match. It's basically a shortcut for
  // starts_with followed by remove_prefix.
  bool Consume(const std::string &x) noexcept {
    if (cur_.StartsWith(x)) {
      size_t sz = x.size();
      cur_.Drop(sz);
      pos_moved_ += sz;
      return true;
    }
    return false;
  }

  // Checks whether const StrSlice & starts with x and if so advances
  // the beginning of it to past the match. It's basically a shortcut for
  // starts_with followed by remove_prefix.
  bool Consume(const StrSlice &x) noexcept {
    if (cur_.StartsWith(x)) {
      size_t sz = x.size();
      cur_.Drop(sz);
      pos_moved_ += sz;
      return true;
    }
    return false;
  }

  void Drop(size_t sz) noexcept {
    cur_.Drop(sz);
    pos_moved_ += sz;
  }

  void ScanUntilImpl(char end_ch, bool escaped) noexcept;

  DISALLOW_COPY_AND_ASSIGN(StrScan);
};

}  // namespace argcv

#endif  // ARGCV_CXX_STR_STR_SCAN_H_
