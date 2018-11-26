licenses(["notice"])  # MIT

exports_files(["Copyright"])


genrule(
    name = "gen_configure",
    srcs = [
        "Makefile.in",
        "config.guess",
        "config.h.in",
        "config.sub",
        "configure",
        "entities.c",
        "install-sh",
        "libxml-2.0.pc.in",
        "libxml-2.0-uninstalled.pc.in",
        "libxml2-config.cmake.in",
        "libxml.spec.in",
        "ltmain.sh",
        "missing",
        "xml2-config.in",
        "doc/Makefile.in",
        "doc/devhelp/Makefile.in",
        "doc/examples/Makefile.in",
        "example/Makefile.in",
        "include/Makefile.in",
        "include/libxml/Makefile.in",
        "include/libxml/xmlversion.h.in",
        "python/Makefile.in",
        "python/setup.py.in",
        "python/tests/Makefile.in",
        "xstc/Makefile.in",
    ],
    outs = [
        "config.h",
        "include/libxml/xmlversion.h",
    ],
    cmd = "./$(location configure) --without-lzma > /dev/null " +
          "&& mv config.h $(location config.h) " +
          "&& mv include/libxml/xmlversion.h $(location include/libxml/xmlversion.h)",
)

cc_library(
    name = "libxml2",
    srcs = glob(
        [
            "*.c",
        ],
        exclude = [
            "test*",
            "run*",
            # trio.c:1230:16: error: use of undeclared identifier 'errno'
            # save_errno = errno;
            "trio.c",
            "xmlcatalog.c",
            "xmllint.c",
        ],
    ),
    hdrs = [
        "config.h",
        "include/libxml/xmlversion.h",
    ] + glob(
        [
            "*.h",
            "include/libxml/*.h",
            "trionan.c",
        ],
        exclude = [
            "include/libxml/xmlversion.h",
        ],
    ),
    copts = [
        "-D_REENTRANT",
        "-DHAVE_CONFIG_H",
        "-w",
    ],
    includes = [
        ".",
        "include",
    ],
    linkopts = select({
        "@%ws%//argcv:darwin": [
            "-liconv",
            # "-ldl",
        ],
        "//conditions:default": [
            # TODO for the porpose of normalize the code
            # it should depends on //thord_party:pthread
            # "-pthread",
            #  ref: http://refspecs.linuxbase.org/LSB_3.1.1/LSB-Core-generic/LSB-Core-generic/libdl.html
            "-ldl",
        ],
    }),
    deps = [
          "//external:zlib",
          "@%ws%//third_party:pthread",
    ],
    visibility = ["//visibility:public"],
)
