.PHONY: build test test-all fmt doc clean

build:
	bazel build //...

test:
	bazel test  --test_output=all //argcv/...

test-all:
	bazel test  --test_output=all //...

fmt:
	cfmt argcv
	cfmt examples
	find argcv | grep BUILD$ | xargs -n 1  buildifier --type=build
	find examples | grep BUILD$ | xargs -n 1  buildifier --type=build
	find tools | grep BUILD$ | xargs -n 1  buildifier --type=build
	find third_party | grep \.bzl$ | xargs -n 1  buildifier --type=bzl
	find tools | grep \.bzl$ | xargs -n 1  buildifier --type=bzl
	ls WORKSPACE  | xargs -n 1  buildifier --type=workspace

doc:
	./.doxygen.sh

clean:
	bazel clean
