cc_library(
    name = "hello_world_lib",
    srcs = glob(
        [
          "hello_world/*.c",
          "hello_world/*.cc",
        ],
        exclude = [
          "hello_world/*_test.cc",
          "hello_world/hello_world.cc",
        ],
    ),
    hdrs = glob(
        [
          "hello_world/*.h",
        ],
        exclude = [
          "hello_world/*_test.h",
          "hello_world/hello_world.h",
        ],
    ),
    deps = [
        "//external:glog",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "hello_world",
    srcs = glob(
        [
          "hello_world/hello_world.cc",
          "hello_world/hello_world.h",
        ],
    ),
    deps = [
        ":hello_world_lib",
        "//external:glog",
    ],
    visibility = ["//visibility:public"],
)



cc_test(
    name = "hello_world_test",
    srcs = glob([
        "hello_world/**/*_test.c",
        "hello_world/**/*_test.cc",
        "hello_world/**/*_test.h",
    ]),
    includes = [
    ],
    deps = [
        "//external:gtest_main",
        ":hello_world_lib"
    ],
)
