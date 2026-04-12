load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "hello_gflags",
    srcs = [
        "main.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "@gflags//:gflags",
        "@glog//:glog",
    ],
)
