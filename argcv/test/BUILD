# Copyright 2021 Yu Jing
load("@rules_cc//cc:defs.bzl", "cc_library")

licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "test",
    srcs = [
        "test.cc",
    ],
    hdrs = [
        "test.h",
    ],
    copts = [
        "-Wno-unused-function",
    ],
    deps = [
        "@glog//:glog",
        "@googletest//:gtest_main",
    ],
)
