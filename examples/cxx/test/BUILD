# Copyright 2019 Yu Jing
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
    ]),
    includes = [
        # ".",
    ],
    deps = [
        "//argcv:test",
        "//external:gtest_main",
    ],
)

cc_test(
    name = "example_bench",
    srcs = glob([
        "**/*_bench.cc",
        "**/*_bench.h",
    ]),
    deps = [
        "//argcv:argcv_test",
        "//external:benchmark_main",
    ],
)
