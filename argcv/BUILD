# Copyright 2019 Yu Jing
licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "argcv",
    srcs = glob(
        [
            "**/*.c",
            "**/*.cc",
        ],
        exclude = [
            "**/*_test.cc",
        ],
    ),
    hdrs = glob(
        [
            "**/*.h",
        ],
        exclude = [
            "**/*_test.h",
        ],
    ),
    deps = [
        # "//argcv/cxx/config:config",
    ],
)

cc_test(
    name = "argcv_test",
    srcs = glob([
        "**/*_test.cc",
        "**/*_test.h",
    ]),
    deps = [
        ":argcv",
        "//argcv/cxx/test:gtest_ext",
        "//external:glog",
    ],
)

cc_binary(
    name = "argcv.so",
    deps = [
      ":argcv",
    ],
    linkshared = 1,
)



# configure settings
# based on https://github.com/tensorflow/tensorflow/blob/master/tensorflow/BUILD
# TODO(yu): new config
# 1. https://docs.bazel.build/versions/master/platforms.html#built-in-constraints-and-platforms
# 2. https://docs.bazel.build/versions/master/be/general.html#config_setting.values
config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "freebsd",
    values = {"cpu": "freebsd"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "windows_msvc",
    values = {"cpu": "x64_windows_msvc"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_x86_64",
    values = {"cpu": "k8"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_ppc64le",
    values = {"cpu": "ppc"},
    visibility = ["//visibility:public"],
)
