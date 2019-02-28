# Copyright 2019 Yu Jing

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
      actual = "@com_github_google_googletest//:gtest",
  )

  # //external:gtest_main
  native.bind(
      name = "gtest_main",
      actual = "@com_github_google_googletest//:gtest_main",
  )

  native.bind(
    name = "benchmark",
    actual = "@com_github_google_benchmark//:benchmark",
  )

  native.bind(
    name = "benchmark_main",
    actual = "@com_github_google_benchmark//:benchmark_main",
  )

  # arq(
  #     name = "com_github_gflags_gflags",
  #     urls = [
  #         "https://github.com/gflags/gflags/archive/v2.2.0.zip",
  #     ],
  #     sha256 = "99f9e8b63ea53e4b23f0fd0f68a6f1e397d5512be36716c17cc75966a90f0d57",
  #     strip_prefix = "gflags-2.2.0",
  #     build_file = clean_dep("//third_party:gflags.BUILD"), # Use Customized Build File
  #     repository = repo_name,
  # )
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
      "https://github.com/google/benchmark/archive/4c2af0788977d3bd900585528c2d08b875b2cd39.zip",
    ],
    sha256 = "d1ab75f9e20c294246a31ff25035c0239d7367804edbf6529ae98eed1f79ddf7",
    strip_prefix = "benchmark-4c2af0788977d3bd900585528c2d08b875b2cd39",
    repository = repo_name,
  )

  arq(
      name = "com_github_google_googletest",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/googletest/archive/release-1.8.0.zip",
          "https://github.com/google/googletest/archive/release-1.8.0.zip",
      ],
      sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
      strip_prefix = "googletest-release-1.8.0",
      build_file = clean_dep("//third_party:gmock.BUILD"),
      repository = repo_name,
  )
