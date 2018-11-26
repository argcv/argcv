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
Original Source: https://goo.gl/SdHW9Z
  aka: tensorflow/core/lib/core/status_test.cc@tensorflow

Revised By: Yu Jing
*/
#include "argcv/cxx/status/status_test.h"

#include "argcv/cxx/status/errors.h"
#include "argcv/cxx/status/status.h"
// #include "argcv/cxx/env/macros.h"

#include "gtest/gtest.h"

namespace argcv {

TEST(Status, OK) {
  EXPECT_EQ(Status::OK().code(), error::OK);
  EXPECT_EQ(Status::OK().error_message(), "");
  EXPECT_OK(Status::OK());
  ASSERT_OK(Status::OK());
  EXPECT_EQ(Status::OK(), Status());
  Status s;
  EXPECT_TRUE(s.ok());
}

TEST(DeathStatus, CheckOK) {
  Status status(errors::InvalidArgument("Invalid"));
  ASSERT_DEATH(CHECK_OK(status), "Invalid");
}

TEST(Status, Set) {
  Status status;
  status = Status(error::CANCELLED, "Error message");
  EXPECT_EQ(status.code(), error::CANCELLED);
  EXPECT_EQ(status.error_message(), "Error message");
}

TEST(Status, Copy) {
  Status a(errors::InvalidArgument("Invalid"));
  Status b(a);
  ASSERT_EQ(a.ToString(), b.ToString());
}

TEST(Status, Assign) {
  Status a(errors::InvalidArgument("Invalid"));
  Status b;
  b = a;
  ASSERT_EQ(a.ToString(), b.ToString());
}

TEST(Status, Update) {
  Status s;
  s.Update(Status::OK());
  ASSERT_TRUE(s.ok());
  Status a(errors::InvalidArgument("Invalid"));
  s.Update(a);
  ASSERT_EQ(s.ToString(), a.ToString());
  Status b(errors::Internal("Internal"));
  s.Update(b);
  ASSERT_EQ(s.ToString(), a.ToString());
  s.Update(Status::OK());
  ASSERT_EQ(s.ToString(), a.ToString());
  ASSERT_FALSE(s.ok());
}

TEST(Status, EqualsOK) { ASSERT_EQ(Status::OK(), Status()); }

TEST(Status, EqualsSame) {
  Status a(errors::InvalidArgument("Invalid"));
  Status b(errors::InvalidArgument("Invalid"));
  ASSERT_EQ(a, b);
}

TEST(Status, EqualsCopy) {
  const Status a(errors::InvalidArgument("Invalid"));
  const Status b = a;
  ASSERT_EQ(a, b);
}

TEST(Status, EqualsDifferentCode) {
  const Status a(errors::InvalidArgument("message"));
  const Status b(errors::Internal("message"));
  ASSERT_NE(a, b);
}

TEST(Status, EqualsDifferentMessage) {
  const Status a(errors::InvalidArgument("message"));
  const Status b(errors::InvalidArgument("another"));
  ASSERT_NE(a, b);
}

TEST(Status, IsErrorMacro) {
  const Status a(errors::InvalidArgument("Invalid"));
  EXPECT_TRUE(errors::IsInvalidArgument(a));
}

TEST(Status, ReturnIfErrorMacro) {
  bool correctly_returned = true;
  const Status a(errors::InvalidArgument("Invalid"));
  [&correctly_returned](const Status &s) -> Status {
    RETURN_IF_ERROR(s);
    correctly_returned = false;
    return Status::OK();
  }(a);
  EXPECT_TRUE(correctly_returned);
}

TEST(Status, ReturnWithContextIfErrorMacro) {
  bool correctly_returned = true;
  const Status a(errors::InvalidArgument("Invalid"));
  const Status r = [&correctly_returned](const Status &s) -> Status {
    RETURN_WITH_CONTEXT_IF_ERROR(s, std::string(" "), "Con", 't', "ext");
    correctly_returned = false;
    return Status::OK();
  }(a);
  EXPECT_TRUE(correctly_returned);
  EXPECT_FALSE(r.ok());
  EXPECT_EQ(error::INVALID_ARGUMENT, r.code());
  EXPECT_EQ("Invalid argument: Invalid\n\t Context", r.ToString());
}

}  // namespace argcv
