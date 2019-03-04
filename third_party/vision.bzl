# Copyright 2019 Yu Jing

load("//tools/build_rules:arq.bzl", "arq", "clean_dep")

def argcv_vision_repositories(repo_name):
  print("argcv_vision_repositories working...")

  native.bind(
    name = "svpng",
    actual = "@com_github_miloyip_svpng//:svpng",
  )

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
