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
        "//external:glog",
        "//external:gtest_main",
    ],
)
