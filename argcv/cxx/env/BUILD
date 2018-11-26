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
    name = "protobuf",
    hdrs = [
        "protobuf.h",
    ],
    deps = [
        "//external:protobuf_compiler",
    ],
)

cc_test(
    name = "protobuf_test",
    srcs = [
      "protobuf_test.cc",
    ],
    deps = [
        ":protobuf",
        "//argcv/cxx:gtest_ext",
        "//external:glog",
        "//external:gtest_main",
    ],
)

cc_library(
    name = "macros",
    hdrs = [
        "macros.h",
    ],
    deps = [
        "//argcv/c:macros",
    ]
)

cc_test(
    name = "macros_test",
    srcs = [
        "macros_test.cc",
    ],
    deps = [
        ":macros",
        "//argcv/cxx:gtest_ext",
        "//external:glog",
        "//external:gtest_main",
    ],
)

cc_library(
    name = "env",
    srcs = [
        "env.cc",
    ],
    hdrs = [
        "env.h",
        "config.h",
    ],
    deps = [
        # ":timer",
        "//external:glog",
    ],
)

cc_test(
    name = "env_test",
    srcs = [
        "env_test.cc",
    ],
    deps = [
        ":env",
        "//argcv/cxx:gtest_ext",
        "//external:glog",
        "//external:gtest_main",
    ],
)

# cc_library(
#     name = "timer",
#     srcs = [
#     ],
#     hdrs = [
#         "timer.h",
#         "ticker.h"
#     ],
#     deps = [
#     ],
# )
#
# cc_test(
#     name = "timer_test",
#     srcs = [
#         "timer_test.cc",
#         "ticker_test.cc",
#     ],
#     deps = [
#         ":env",
#         "//argcv/cxx:gtest_ext",
#         "//external:glog",
#         "//external:gtest_main",
#     ],
# )

genrule(
    name = "config_h",
    srcs = [
        "config.h.template",
    ],
    outs = [
        "config.h",
    ],
    cmd = "cat $< | $(location //tools:configure_file) > $@",
    local = 1,
    tools = [
      "//tools:configure_file",
    ],
)

