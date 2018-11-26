# The MIT License (MIT)
#
# Copyright (c) 2018 Yu Jing
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "proto",
    deps = [
      ":proto_proto",
    ],
)

cc_proto_library(
    name = "api",
    deps = [
      ":api_proto",
    ],
)

cc_proto_library(
    name = "status",
    deps = [
      ":status_proto",
    ],
)

cc_proto_library(
    name = "type",
    deps = [
      ":type_proto",
    ],
)

proto_library(
    name = "api_proto",
    deps = [
      "//argcv/proto/api:annotations_proto",
      "//argcv/proto/api:http_proto",
      "//argcv/proto/api:httpbody_proto",
      "//argcv/proto/api:label_proto",
    ],
)

proto_library(
    name = "longrunning_proto",
    deps = [
      "//argcv/proto/longrunning:longrunning_proto",
    ],
)

proto_library(
    name = "status_proto",
    deps = [
      "//argcv/proto/status:error_codes_proto",
      "//argcv/proto/status:error_details_proto",
      "//argcv/proto/status:status_proto",
    ],
)

proto_library(
    name = "type_proto",
    deps = [
      "//argcv/proto/type:latlng_proto",
    ],
)

proto_library(
    name = "proto_proto",
    deps = [
      ":api_proto",
      ":status_proto",
      ":type_proto",
    ],
)
