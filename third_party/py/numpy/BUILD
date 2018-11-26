licenses(["restricted"])

# http://stackoverflow.com/questions/36624426/bazel-create-py-binary-from-python-file-in-py-library
# Bazel: create py_binary from python file in py_library

package(default_visibility = ["//visibility:public"])

py_library(
    name = "numpy",
    srcs = ["numpy_dummy.py"],
    srcs_version = "PY2AND3",
)

cc_library(
    name = "headers",
    hdrs = glob(["numpy_include/**/*.h"]),
    data = ["//third_party/py:python_checked"],
    includes = ["numpy_include"],
)

genrule(
    name = "dummy",
    outs = ["numpy_dummy.py"],
    cmd = "touch $@",
    visibility = ["//visibility:private"],
)
