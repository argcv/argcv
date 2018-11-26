package(default_visibility = ["//visibility:public"])


filegroup(
    name = "licenses",
    srcs = glob([
        "**/LICENSE",
        "**/COPY",
        "**/LICENSE.md",
    ]),
)


cc_library(
    name = "pthread",
    linkopts = [
    ] + select({
        "//argcv:darwin": [
            # Darwin does not require to depends on pthread
            # In clang
        ],
        "//conditions:default": [
            "-pthread",
        ],
    }),
)