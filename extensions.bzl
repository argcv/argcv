load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def _non_bcr_deps_impl(_mctx):
    http_archive(
        name = "com_github_miloyip_svpng",
        urls = [
            "https://github.com/miloyip/svpng/archive/cdb4acba06cb105700195d044c8dd4561bf9b278.zip",
        ],
        build_file = "//third_party:svpng.BUILD",
        sha256 = "fa23e054e89e37bd5d9845adde83dc4651c08b82de66da4bd42fe44a0ab48dd6",
        strip_prefix = "svpng-cdb4acba06cb105700195d044c8dd4561bf9b278",
    )

    http_archive(
        name = "com_github_nothings_stb",
        urls = [
            "https://github.com/nothings/stb/archive/72990efc3e4e27fbd4b1130bb11353ef7ea2e2e6.zip",
        ],
        build_file = "//third_party:stb.BUILD",
        sha256 = "40afff77fc1b9913fec01d6da00c3f902197ccff60e3c5be99a70ca562613d63",
        strip_prefix = "stb-72990efc3e4e27fbd4b1130bb11353ef7ea2e2e6",
    )

non_bcr_deps = module_extension(
    implementation = _non_bcr_deps_impl,
)
