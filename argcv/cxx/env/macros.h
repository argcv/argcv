/* Copyright 2015 Google Inc. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

This file is from:
https://github.com/petewarden/tensorflow_makefile/blob/master/tensorflow/core/platform/macros.h
==============================================================================*/
#ifndef ARGCV_CXX_ENV_MACROS_H_
#define ARGCV_CXX_ENV_MACROS_H_

#include "argcv/c/macros.h"

// GCC can be told that a certain branch is not likely to be taken (for
// instance, a CHECK failure), and use that information in static analysis.
// Giving it this information can help it optimize for the common case in
// the absence of better information (ie. -fprofile-arcs).
#if defined(COMPILER_GCC3)
#define PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define PREDICT_FALSE(x) (x)
#define PREDICT_TRUE(x) (x)
#endif  //  defined(COMPILER_GCC3)

// A macro to disallow the copy constructor and operator= functions
// This is usually placed in the private: declarations for a class.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName &) = delete;     \
  void operator=(const TypeName &) = delete

// The ARRAYSIZE(arr) macro returns the # of elements in an array arr.
//
// The expression ARRAYSIZE(a) is a compile-time constant of type
// size_t.
#define ARRAYSIZE(a)            \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))  // NOLINT

#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
// Define this to 1 if the code is compiled in C++11 mode; leave it
// undefined otherwise.  Do NOT define it to 0 -- that causes
// '#ifdef LANG_CXX11' to behave differently from '#if LANG_CXX11'.
#define LANG_CXX11 1
#endif  //  defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L

#if defined(__clang__) && defined(LANG_CXX11) && defined(__has_warning)
#if __has_feature(cxx_attributes) && __has_warning("-Wimplicit-fallthrough")
#define FALLTHROUGH_INTENDED [[clang::fallthrough]]  // NOLINT
#endif  //  __has_feature(cxx_attributes) &&
        //  __has_warning("-Wimplicit-fallthrough")
#endif  //  defined(__clang__) && defined(LANG_CXX11) && defined(__has_warning)

#ifndef FALLTHROUGH_INTENDED
#define FALLTHROUGH_INTENDED \
  do {                       \
  } while (0)
#endif  // FALLTHROUGH_INTENDED

// A macro to enable compare related operators
// This is usually placed in the public: declarations for a class.
#define OP_COMPARABLE(TypeName)                          \
  bool operator==(const TypeName &that) const noexcept { \
    return this->Compare(that) == 0;                     \
  }                                                      \
  bool operator!=(const TypeName &that) const noexcept { \
    return this->Compare(that) != 0;                     \
  }                                                      \
  bool operator<(const TypeName &that) const noexcept {  \
    return this->Compare(that) < 0;                      \
  }                                                      \
  bool operator>(const TypeName &that) const noexcept {  \
    return this->Compare(that) > 0;                      \
  }                                                      \
  bool operator<=(const TypeName &that) const noexcept { \
    return this->Compare(that) <= 0;                     \
  }                                                      \
  bool operator>=(const TypeName &that) const noexcept { \
    return this->Compare(that) >= 0;                     \
  }

#endif  // ARGCV_CXX_ENV_MACROS_H_
