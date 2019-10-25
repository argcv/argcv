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

doc:
	./.doxygen.sh

clean:
	bazel clean
