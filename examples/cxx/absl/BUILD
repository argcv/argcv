cc_binary(
    name = "hello-absl",
    srcs = [
        "main.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//external:absl/strings",
        "//external:glog",
    ],
)

# see: https://github.com/bazelbuild/bazel/tree/master/tools/build_defs/pkg
load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar", "pkg_deb", "pkg_rpm")

pkg_tar(
    name = "hello-absl-bin",
    srcs = [
        "//examples/cxx/absl:hello-absl",
    ],
    mode = "0755",
    package_dir = "/tmp/example",
    strip_prefix = "/examples/cxx/absl/",
)
