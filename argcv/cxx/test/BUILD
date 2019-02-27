# Copyright 2019 Yu Jing
licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "gtest_ext",
    hdrs = [
        "gtest_ext.h",
    ],
    deps = [
        "//external:gtest_main",
    ],
)

cc_test(
    name = "gtest_ext_test",
    srcs = glob([
        "*_test.cc",
        "*_test.h",
    ]),
    copts = [
        "-Wno-unused-function",
    ],
    deps = [
        ":gtest_ext",
        "//external:glog",
    ],
)
