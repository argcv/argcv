# Copyright 2021 Yu Jing
load("//tools/build_rules:arq.bzl", "arq", "clean_dep")

def argcv_infra_repositories(repo_name):
    print("argcv_infra_repositories working...")

    native.bind(
        name = "gflags",
        actual = "@com_github_gflags_gflags//:gflags",
    )

    native.bind(
        name = "glog",
        actual = "@com_github_google_glog//:glog",
    )

    # //external:gtest
    native.bind(
        name = "gtest",
        actual = "@com_google_googletest//:gtest",
    )

    # //external:gtest_main
    native.bind(
        name = "gtest_main",
        actual = "@com_google_googletest//:gtest_main",
    )

    native.bind(
        name = "benchmark",
        actual = "@com_github_google_benchmark//:benchmark",
    )

    native.bind(
        name = "benchmark_main",
        actual = "@com_github_google_benchmark//:benchmark_main",
    )

    # please see https://github.com/abseil/abseil-cpp/tree/master/absl
    # for other packages
    native.bind(
        name = "absl/strings",
        actual = "@com_google_absl//absl/strings",
    )

    # native.bind(
    #   name = "absl/algorithm",
    #   actual = "@com_google_absl//absl/algorithm",
    # )

    # native.bind(
    #   name = "absl/base",
    #   actual = "@com_google_absl//absl/algorithm",
    # )

    arq(
        name = "bazel_toolchains",
        urls = [
            "https://mirror.bazel.build/github.com/bazelbuild/bazel-toolchains/archive/bc09b995c137df042bb80a395b73d7ce6f26afbe.tar.gz",
            "https://github.com/bazelbuild/bazel-toolchains/archive/bc09b995c137df042bb80a395b73d7ce6f26afbe.tar.gz",
        ],
        sha256 = "4329663fe6c523425ad4d3c989a8ac026b04e1acedeceb56aa4b190fa7f3973c",
        strip_prefix = "bazel-toolchains-bc09b995c137df042bb80a395b73d7ce6f26afbe",
        repository = repo_name,
    )

    arq(
        name = "com_github_gflags_gflags",
        urls = [
            "https://github.com/gflags/gflags/archive/v2.2.2.zip",
        ],
        sha256 = "19713a36c9f32b33df59d1c79b4958434cb005b5b47dc5400a7a4b078111d9b5",
        strip_prefix = "gflags-2.2.2",
        repository = repo_name,
    )

    arq(
        name = "com_github_google_glog",
        urls = [
            "https://github.com/google/glog/archive/da816ea70645e463aa04f9564544939fa327d5a7.zip",
        ],
        build_file = clean_dep("//third_party:glog.BUILD"),
        sha256 = "b2f35994772423a6f51e885e57a345350bac70a5aee8e035428643c0a0a4ca30",
        strip_prefix = "glog-da816ea70645e463aa04f9564544939fa327d5a7",
        repository = repo_name,
    )

    arq(
        name = "com_github_google_benchmark",
        urls = [
            "https://github.com/google/benchmark/archive/v1.5.5.zip",
        ],
        sha256 = "30f2e5156de241789d772dd8b130c1cb5d33473cc2f29e4008eab680df7bd1f0",
        strip_prefix = "benchmark-1.5.5",
        repository = repo_name,
    )

    arq(
        name = "com_google_googletest",
        urls = [
            "http://bazel-mirror.storage.googleapis.com/github.com/google/googletest/archive/release-1.8.0.zip",
            "https://github.com/google/googletest/archive/release-1.8.0.zip",
        ],
        sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
        strip_prefix = "googletest-release-1.8.0",
        build_file = clean_dep("//third_party:gmock.BUILD"),
        repository = repo_name,
    )

    arq(
        name = "com_google_absl",
        urls = [
            "https://github.com/abseil/abseil-cpp/archive/419f3184f8ebcdb23105295eadd2a569f3351eb9.zip",
        ],
        sha256 = "318a6e11de01dbda14e3d587e08c691dd0b7b7e2a43030cd3f02917135933acb",
        strip_prefix = "abseil-cpp-419f3184f8ebcdb23105295eadd2a569f3351eb9",
        repository = repo_name,
    )
