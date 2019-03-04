# Copyright 2019 Yu Jing

# How to prepare rules: https://bazel.build/versions/master/docs/skylark/cookbook.html
load("//tools/build_rules/config:system.bzl", "cc_system_package")
load("//tools/build_rules:arq.bzl", "arq", "clean_dep")
load("//third_party:infra.bzl", "argcv_infra_repositories")

def argcv_repositories(repo_name="com_github_argcv_argcv"):
  print("argcv_repositories working...")
  argcv_infra_repositories(repo_name)

  arq(
      name = "com_github_miloyip_svpng",
      urls = [
          "https://github.com/miloyip/svpng/archive/cdb4acba06cb105700195d044c8dd4561bf9b278.zip",
      ],
      build_file = clean_dep("//third_party:svpng.BUILD"),
      sha256 = "fa23e054e89e37bd5d9845adde83dc4651c08b82de66da4bd42fe44a0ab48dd6",
      strip_prefix = "svpng-cdb4acba06cb105700195d044c8dd4561bf9b278",
      repository = repo_name,
  )

  arq(
      name = "com_github_nothings_stb",
      urls = [
          "https://github.com/nothings/stb/archive/72990efc3e4e27fbd4b1130bb11353ef7ea2e2e6.zip",
      ],
      build_file = clean_dep("//third_party:stb.BUILD"),
      sha256 = "40afff77fc1b9913fec01d6da00c3f902197ccff60e3c5be99a70ca562613d63",
      strip_prefix = "stb-72990efc3e4e27fbd4b1130bb11353ef7ea2e2e6",
      repository = repo_name,
  )
