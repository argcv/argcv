# Bazel build file for gflags
licenses(["notice"])

cc_library(
    name = "gflags",
    srcs = [
        "src/gflags.cc",
        "src/gflags_completions.cc",
        "src/gflags_reporting.cc",
        "src/mutex.h",
        "src/util.h",
        ":config_h",
        ":gflags_completions_h",
        ":gflags_declare_h",
        ":gflags_h",
        ":includes",
    ],
    hdrs = ["include/gflags/gflags.h"],
    copts = [
        "-I$(GENDIR)/external/com_github_gflags_gflags/src",
        "-Wno-sign-compare",
        "-DHAVE_STDINT_H",
        "-DHAVE_SYS_TYPES_H",
        "-DHAVE_INTTYPES_H",
        "-DHAVE_SYS_STAT_H",
        "-DHAVE_UNISTD_H",
        "-DHAVE_FNMATCH_H",
        "-DHAVE_STRTOLL",
        "-DHAVE_STRTOQ",
        "-DHAVE_PTHREAD",
        "-DHAVE_RWLOCK",
        "-DGFLAGS_INTTYPES_FORMAT_C99",
    ],
    linkopts = [
        "-lpthread",
    ],
    includes = [
        "include",
    ],
    visibility = ["//visibility:public"],
)

genrule(
    name = "config_h",
    srcs = [
        "src/config.h.in",
    ],
    outs = [
        "src/config.h",
    ],
    cmd = "awk '{ gsub(/^#cmakedefine/, \"//cmakedefine\"); print; }' $(<) > $(@)",
)

genrule(
    name = "gflags_h",
    srcs = [
        "src/gflags.h.in",
    ],
    outs = [
        "src/gflags.h",
    ],
    cmd = "awk '{ gsub(/@(GFLAGS_ATTRIBUTE_UNUSED|INCLUDE_GFLAGS_NS_H)@/, \"\"); print; }' $(<) > $(@)",
)

genrule(
    name = "gflags_completions_h",
    srcs = [
        "src/gflags_completions.h.in",
    ],
    outs = [
        # note: this change is for new gflags v2.2.0
        # "src/gflags_completions.h",
        "gflags/gflags_completions.h",
    ],
    cmd = "awk '{ gsub(/@GFLAGS_NAMESPACE@/, \"gflags\"); print; }' $(<) > $(@)",
)

genrule(
    name = "gflags_declare_h",
    srcs = [
        "src/gflags_declare.h.in",
    ],
    outs = [
        "src/gflags_declare.h",
    ],
    cmd = ("awk '{ " +
           "gsub(/@GFLAGS_NAMESPACE@/, \"gflags\"); " +
           "gsub(/@(HAVE_STDINT_H|HAVE_SYS_TYPES_H|HAVE_INTTYPES_H|GFLAGS_INTTYPES_FORMAT_C99)@/, \"1\"); " +
           "gsub(/@([A-Z0-9_]+)@/, \"0\"); " +
           "print; }' $(<) > $(@)"),
)

genrule(
    name = "includes",
    srcs = [
        ":gflags_h",
        ":gflags_declare_h",
    ],
    outs = [
        "include/gflags/gflags.h",
        "include/gflags/gflags_declare.h",
    ],
    cmd = "mkdir -p $(@D)/include/gflags && cp $(SRCS) $(@D)/include/gflags",
)
