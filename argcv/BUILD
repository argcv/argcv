# The MIT License (MIT)
#
# Copyright (c) 2018 Yu Jing
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
licenses(["notice"])  # MIT

package(default_visibility = ["//visibility:public"])

# cc_library(
#     name = "c",
#     # srcs = [
#     #   "cxx/empty.cc",
#     # ],
#     deps = [
#       "//argcv/c:sys",
#       "//argcv/c:char",
#     ],
# )

cc_library(
    name = "cc",
    # srcs = [
    #   "cxx/empty.cc",
    # ],
    deps = [
      "//argcv/cxx:par",
      "//argcv/cxx:net",
      "//argcv/cxx:math",
      "//argcv/cxx:str",
    ],
)

cc_binary(
    name = "argcv.so",
    deps = [
      # ":c",
      ":cc",
    ],
    linkshared = 1,
)



# configure settings
# based on https://github.com/tensorflow/tensorflow/blob/master/tensorflow/BUILD
# TODO(yu): new config
# 1. https://docs.bazel.build/versions/master/platforms.html#built-in-constraints-and-platforms
# 2. https://docs.bazel.build/versions/master/be/general.html#config_setting.values
config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "freebsd",
    values = {"cpu": "freebsd"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "windows_msvc",
    values = {"cpu": "x64_windows_msvc"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_x86_64",
    values = {"cpu": "k8"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_ppc64le",
    values = {"cpu": "ppc"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "debug",
    values = {
        "compilation_mode": "dbg",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "optimized",
    values = {
        "compilation_mode": "opt",
    },
    visibility = ["//visibility:public"],
)

# TODO(jhseu): Enable on other platforms other than Linux.
config_setting(
    name = "with_jemalloc_linux_x86_64",
    values = {
        "cpu": "k8",
        "define": "with_jemalloc=true",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_jemalloc_linux_ppc64le",
    values = {
        "cpu": "ppc",
        "define": "with_jemalloc=true",
    },
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_gcp_support",
    values = {"define": "with_gcp_support=true"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_hdfs_support",
    values = {"define": "with_hdfs_support=true"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_xla_support",
    values = {"define": "with_xla_support=true"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_verbs_support",
    values = {"define": "with_verbs_support=true"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "with_mpi_support",
    values = {"define": "with_mpi_support=true"},
    visibility = ["//visibility:public"],
)

