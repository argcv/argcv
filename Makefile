.PHONY: build setup clean

build:
	bazel build //...

setup:
	./setup

clean:
	bazel clean
