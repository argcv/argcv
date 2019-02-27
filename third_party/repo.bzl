# Copyright 2019 Yu Jing

# How to prepare rules: https://bazel.build/versions/master/docs/skylark/cookbook.html
load("//tools/build_rules/config:system.bzl", "cc_system_package")
load("//tools/build_rules:arq.bzl", "arq")
load("//third_party:infra.bzl", "argcv_infra_workspace")

def argcv_repositories(repo_name="com_github_argcv_argcv"):
  print("argcv_workspace working...")
  argcv_infra_workspace(repo_name)

