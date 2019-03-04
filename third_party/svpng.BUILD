# Description:
#   Minimalistic C function for saving RGB/RGBA image as uncompressed PNG.

licenses(["notice"])  # 3-clause BSD

exports_files(["LICENSE"])

cc_library(
    name = "svpng",
    hdrs = [
        "svpng.inc",
    ],
    copts = [
    ],
    linkopts = [
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "example",
    srcs = [
        "example.c",
    ],
    copts = [],
    deps = [
        ":svpng",
    ],
)

