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

cc_library(
    name = "ccurl",
    hdrs = [
      "ccurl.h",
    ],
    deps = [
      "//external:curl",
      "//argcv/cxx:str",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "request",
    hdrs = [
      "request.h",
    ],
    srcs = [
      "request.cc",
    ],
    deps = [
      ":ccurl",
      "//argcv/cxx:env",
      "//argcv/cxx:status",
      "//external:glog",
      "//argcv/cxx:str",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "request_test",
    srcs = [
      "request_test.cc",
    ],
    deps = [
        ":request",
        "//argcv/cxx:gtest_ext",
        "//external:glog",
        "//external:gtest_main",
    ],
)