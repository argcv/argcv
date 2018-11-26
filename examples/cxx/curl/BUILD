cc_binary(
    name = "hello_curl",
    srcs = [
          "hello_curl.cc",
    ],
    deps = [
        "//external:glog",
        "//external:curl",
    ],
    visibility = ["//visibility:public"],
)


cc_binary(
    name = "hello_request",
    srcs = [
          "hello_request.cc",
    ],
    deps = [
        "//argcv/cxx/net/http:request",
        "//external:glog",
        "//external:curl",
    ],
    visibility = ["//visibility:public"],
)