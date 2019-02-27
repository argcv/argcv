# Copyright 2019 Yu Jing
load(
    "//third_party:repo.bzl",
    _argcv_repositories = "argcv_repositories",
)
load(
    "//tools/build_rules:arq.bzl",
    _arq = "arq",
)

argcv_repositories = _argcv_repositories

arq = _arq
