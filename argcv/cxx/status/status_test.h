/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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
Original Source: https://goo.gl/CsXWVx
  aka: tensorflow/core/lib/core/status_test_util.h@tensorflow

Revised By: Yu Jing
*/

#ifndef ARGCV_CXX_STATUS_STATUS_TEST_H_
#define ARGCV_CXX_STATUS_STATUS_TEST_H_

#include "argcv/cxx/status/errors.h"
#include "argcv/cxx/status/status.h"

#include "gtest/gtest.h"

// Macros for testing the results of functions that return tensorflow::Status.
#define EXPECT_OK(val) EXPECT_EQ(::argcv::Status::OK(), (val))
#define ASSERT_OK(val) ASSERT_EQ(::argcv::Status::OK(), (val))

#endif  // ARGCV_CXX_STATUS_STATUS_TEST_H_
