# Description:
#   RapidJSON is A fast JSON parser/generator for C++ with both SAX/DOM style API

licenses(["notice"])  # MIT

filegroup(
    name = "license",
    srcs = ["license.txt"],
)

cc_library(
    name = "rapidjson",
    hdrs = glob([
        "include/**/*.h",
    ]),
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)
