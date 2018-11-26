"""This file is intended to create a few functions to help you
create *.grpc.pb.h based on cc_proto_library

**WIP**

From https://github.com/bazelbuild/bazel/blob/master/tools/build_rules/genproto.bzl

# Copyright 2014 The Bazel Authors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
proto_filetype = [".proto"]

def cc_grpc_library(name, src):
  basename = src[:-len(".proto")]
  protoc_label = str(Label("//external:protoc"))
  cpp_plugin_label = str(Label("//external:grpc_cpp_plugin"))
  native.genrule(
      name = name + "_codegen",
      srcs = [src],
      tools = [protoc_label, cpp_plugin_label],
      cmd = "\\\n".join([
          "$(location " + protoc_label + ")",
          "    --plugin=protoc-gen-grpc=$(location " + cpp_plugin_label + ")",
          "    --cpp_out=$(GENDIR)",
          "    --grpc_out=$(GENDIR)",
          "    $(location " + src + ")"]),
      outs = [basename + ".grpc.pb.h", basename + ".grpc.pb.cc", basename + ".pb.cc", basename + ".pb.h"])

  native.cc_library(
      name = name,
      srcs = [basename + ".grpc.pb.cc", basename + ".pb.cc"],
      hdrs = [basename + ".grpc.pb.h", basename + ".pb.h"],
      deps = [str(Label("//external:grpc_lib"))],
      includes = ["."])

