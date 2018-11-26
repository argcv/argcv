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
    name = "gtest_ext",
    hdrs = [
        "test/gtest_ext.h",
    ],
    deps = [
        "//external:gtest_main",
    ],
)

cc_test(
    name = "gtest_ext_test",
    srcs = glob([
        "test/*_test.cc",
        "test/*_test.h",
    ]),
    copts = [
        "-Wno-unused-function",
    ],
    deps = [
        ":gtest_ext",
        "//external:glog",
    ],
)

cc_library(
    name = "par",
    srcs = glob(
        [
            "par/*.c",
            "par/*.cc",
        ],
        exclude = [
            "par/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "par/*.h",
        ],
        exclude = [
            "par/*_test.h",
        ],
    ),
    deps = [
        ":math",
        ":type",
        "//argcv/cxx/env:env",
        "//argcv/cxx/env:macros",
    ],
)

cc_test(
    name = "par_test",
    srcs = glob([
        "par/*_test.cc",
        "par/*_test.h",
    ]),
    copts = [
        "-Wno-unused-function",
    ],
    deps = [
        ":par",
        ":gtest_ext",
        "//external:glog",
        "//external:gtest_main",
    ],
)

cc_library(
    name = "net",
    srcs = glob(
        [
            "net/**/*.c",
            "net/**/*.cc",
        ],
        exclude = [
            "net/**/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "net/**/*.h",
        ],
        exclude = [
            "net/**/*_test.h",
        ],
    ),
    deps = [
        ":par",
        "//external:curl",
        "//external:glog",
    ],
)

cc_test(
    name = "net_test",
    srcs = glob([
        "net/**/*_test.cc",
        "net/**/*_test.h",
    ]),
    deps = [
        "//external:glog",
        "//external:gtest_main",
        ":net",
        # ":gtest_ext",
    ],
)

cc_library(
    name = "math",
    srcs = glob(
        [
            "math/**/*.c",
            "math/**/*.cc",
        ],
        exclude = [
            "math/**/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "math/**/*.h",
        ],
        exclude = [
            "math/**/*_test.h",
        ],
    ),
)

cc_test(
    name = "math_test",
    srcs = glob([
        "math/**/*_test.cc",
        "math/**/*_test.h",
    ]),
    deps = [
        "//external:glog",
        "//external:gtest_main",
        ":math",
        # ":gtest_ext",
    ],
)


cc_library(
    name = "str",
    srcs = glob(
        [
            "str/*.c",
            "str/*.cc",
        ],
        exclude = [
            "str/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "str/*.h",
        ],
        exclude = [
            "str/*_test.h",
        ],
    ),
    deps = [
        ":math",
        ":type",
        "//argcv/c:char",
        "//argcv/cxx/env:macros",
        "//external:glog",
        "//third_party/porter2_stemmer",
    ],
)

cc_test(
    name = "str_test",
    srcs = glob([
        "str/*_test.cc",
        "str/*_test.h",
    ]),
    deps = [
        "//external:glog",
        "//external:gtest_main",
        ":str",
        # ":gtest_ext",
    ],
)

cc_library(
    name = "status",
    deps = [
      "//argcv/cxx/status:status",
    ],
)

cc_library(
    name = "type",
    srcs = glob(
        [
            "type/*.c",
            "type/*.cc",
        ],
        exclude = [
            "type/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "type/*.h",
        ],
        exclude = [
            "type/*_test.h",
        ],
    ),
    deps = [
        "//argcv/cxx/env:macros",
    ],
)

cc_test(
    name = "type_test",
    srcs = glob([
        "type/*_test.cc",
        "type/*_test.h",
    ]),
    deps = [
        "//external:glog",
        "//external:gtest_main",
        ":type",
    ],
)

# Note: Add Whole Sub-Directory As Source Is Easy
# However, Some Dependencies Are Complex In Platform
# We are supposed to Add Them Separately
cc_library(
    name = "env",
    deps = [
      "//argcv/cxx/env:env",
      "//argcv/cxx/env:macros",
      "//argcv/cxx/env:protobuf",
    ],
)

cc_library(
    name = "encoding",
    deps = [
      "//argcv/cxx/encoding:xml",
    ],
)

