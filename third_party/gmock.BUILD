# Description:
#   Google C++ Mocking Framework, a library for creating and using C++
#   mock classes.

licenses(["notice"])  # 3-clause BSD

exports_files(["LICENSE"])

cc_library(
    name = "gtest",
    srcs = [
        "googlemock/src/gmock-all.cc",
        "googletest/src/gtest-all.cc",
    ],
    hdrs = glob([
        "**/*.h",
        "googletest/src/*.cc",
        "googlemock/src/*.cc",
    ]),
    includes = [
        "googlemock",
        "googlemock/include",
        "googletest",
        "googletest/include",
    ],
    copts = [
      "-Wno-deprecated-declarations",
      "-Wno-unused-function",
      "-Wno-unused-command-line-argument",
    ],
    linkopts = [
      "-pthread",
      "-Wno-unused-command-line-argument", # clang does not need pthread
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "gtest_main",
    srcs = ["googlemock/src/gmock_main.cc"],
    copts = [
      "-Wno-deprecated-declarations",
      "-Wno-unused-function",
      "-Wno-unused-command-line-argument",
    ],
    linkopts = [
      "-pthread",
      "-Wno-unused-command-line-argument", # clang does not need pthread
    ],
    visibility = ["//visibility:public"],
    deps = [":gtest"],
)
