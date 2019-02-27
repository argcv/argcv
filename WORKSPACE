workspace(name = "com_github_argcv_argcv")

# # ref: https://github.com/bazelbuild/rules_closure
# # ref: https://github.com/tensorflow/tensorflow/blob/master/tensorflow/workspace.bzl
# http_archive(
#     name = "io_bazel_rules_closure",
#     sha256 = "60fc6977908f999b23ca65698c2bb70213403824a84f7904310b6000d78be9ce",
#     strip_prefix = "rules_closure-5ca1dab6df9ad02050f7ba4e816407f88690cf7d",
#     urls = [
#         "http://bazel-mirror.storage.googleapis.com/github.com/bazelbuild/rules_closure/archive/5ca1dab6df9ad02050f7ba4e816407f88690cf7d.tar.gz",  # 2017-02-03
#         "https://github.com/bazelbuild/rules_closure/archive/5ca1dab6df9ad02050f7ba4e816407f88690cf7d.tar.gz",
#     ],
# )
# load("@io_bazel_rules_closure//closure:defs.bzl", "closure_repositories")
# closure_repositories()

load("//tools:version.bzl", "check_version")
load("//third_party:workspace.bzl", "argcv_workspace")

check_version("0.12.0")

argcv_workspace(repo_name="com_github_argcv_argcv")

