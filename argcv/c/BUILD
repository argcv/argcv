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
    name = "macros",
    hdrs = [
        "macros.h",
        "daemon.h",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dir",
    srcs = [
        "dir/dir.c",
    ],
    hdrs = [
        "dir/dir.h",
    ],
    copts = [
    ],
    deps = [
        # ":macros",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "char",
    srcs = glob(
        [
          "char/*.c",
          # "char/*.cc",
        ],
        exclude = [
          "char/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
          "char/*.h",
        ],
        exclude = [
          "char/*_test.h",
        ],
    ),
    visibility = ["//visibility:public"],
)


# cc_test(
#     name = "sys_test",
#     # srcs = ["example.cc"],
#     # -I is not required here
#     # copts = ["-Iexternal/gmock_archive/googletest/include"],
#     srcs = glob([
#         "sys/*_test.cc",
#         "sys/*_test.h",
#     ]),
#     deps = [
#         "//external:gtest_main",
#         ":sys",
#     ],
# )

cc_test(
    name = "char_test",
    # srcs = ["example.cc"],
    # -I is not required here
    # copts = ["-Iexternal/gmock_archive/googletest/include"],
    srcs = glob([
        "char/*_test.cc",
        "char/*_test.h",
    ]),
    deps = [
        "//external:gtest_main",
        ":char",
    ],
)



