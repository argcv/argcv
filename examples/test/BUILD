# Copyright 2021 Yu Jing
load("@rules_cc//cc:defs.bzl", "cc_test")

licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

# https://bazel.build/versions/master/docs/be/c-cpp.html#cc_test
cc_test(
    name = "example_test",
    # srcs = ["example.cc"],
    # -I is not required here
    # copts = ["-Iexternal/gmock_archive/googletest/include"],
    srcs = glob([
        "**/*_test.cc",
        "**/*_test.h",
    ], allow_empty = True),
    includes = [
        # ".",
    ],
    deps = [
        "//argcv/test",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "example_bench",
    srcs = glob([
        "**/*_bench.cc",
        "**/*_bench.h",
    ], allow_empty = True),
    deps = [
        "@google_benchmark//:benchmark_main",
    ],
)
