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
https://github.com/petewarden/tensorflow_makefile/blob/master/tensorflow/core/platform/protobuf.h
==============================================================================*/
#ifndef ARGCV_CXX_ENV_PROTOBUF_H_
#define ARGCV_CXX_ENV_PROTOBUF_H_

#include <string>

#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/map.h"
#include "google/protobuf/repeated_field.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/util/json_util.h"
#include "google/protobuf/util/type_resolver_util.h"

namespace argcv {
namespace protobuf = ::google::protobuf;

inline static bool ParseProtoUnlimited(protobuf::MessageLite* proto,
                                       const void* serialized, size_t size) {
  protobuf::io::CodedInputStream coded_stream(
      reinterpret_cast<const uint8_t*>(serialized), size);
  coded_stream.SetTotalBytesLimit(INT_MAX, INT_MAX);
  return proto->ParseFromCodedStream(&coded_stream);
}

// Parses a protocol buffer contained in a string in the binary wire format.
// Returns true on success. Note: Unlike protobuf's builtin ParseFromString,
// this function has no size restrictions on the total size of the encoded
// protocol buffer.
inline static bool ParseProtoUnlimited(protobuf::MessageLite* proto,
                                       const std::string& serialized) {
  return ParseProtoUnlimited(proto, serialized.data(), serialized.size());
}

// Returns the string value for the value of a string or bytes protobuf field.
inline const std::string& ProtobufStringToString(const std::string& s) {
  return s;
}

// Set <dest> to <src>. Swapping is allowed, as <src> does not need to be
// preserved.
inline void SetProtobufStringSwapAllowed(std::string* src, std::string* dest) {
  dest->swap(*src);
}

}  // namespace argcv

#endif  // ARGCV_CXX_ENV_PROTOBUF_H_
