#!/usr/bin/env bash
if hash bazel 2>/dev/null ; then
  bazel clean
else
  rm -rf bazel-*
fi

docker build -t argcv .

