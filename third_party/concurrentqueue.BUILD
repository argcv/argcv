# Description:
# concurrentqueue is a fast multi-producer, 
# multi-consumer lock-free concurrent queue for C++11

licenses(["notice"])  # Simplified BSD License

exports_files(["LICENSE.md"])

cc_library(
    name = "concurrentqueue",
    srcs = [
        # empty
    ],
    hdrs = [
        "blockingconcurrentqueue.h",
        "concurrentqueue.h",
    ],
    deps = [
        "@%ws%//third_party:pthread",
    ],
    visibility = ["//visibility:public"],
)
