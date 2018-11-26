# Description:
#     A YAML parser and emitter in C++

licenses(["notice"])  # MIT

exports_files(["LICENSE"])

cc_library(
    name="yaml",
    hdrs=glob(["include/**/*.h", "include/*.h"]),
    srcs=glob(["src/**/*.h", "src/*.h", "src/**/*.cpp", "src/*.cpp"]),
    includes=["include"],
    visibility=["//visibility:public"]
)
