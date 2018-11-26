cc_binary(
    name = "hello_leveldb",
    srcs = [
          "hello_leveldb.cc",
    ],
    deps = [
        "//external:glog",
        "//argcv/cxx/db/leveldb:helper",
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "hello_snappy",
    srcs = [
          "hello_snappy.cc",
    ],
    deps = [
        "//external:glog",
        "//external:snappy",
    ],
    visibility = ["//visibility:public"],
)
