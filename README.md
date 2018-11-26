# argcv

[![Join the chat at https://gitter.im/argcv/argcv][badge-gitter]][link-gitter]
[![Build Status][badge-travis]][link-travis]
[![MIT License][badge-license]](LICENSE)

## Motivation

I wish to get a small scalable platform based on [C++](https://en.wikipedia.org/wiki/C%2B%2B) and then focus on the target I wish to accomplish. 

Inspired by the design of [bazel](https://bazel.build/), I could compile the required code only in high performance, I wish to prepare a base project to write C++ on [bazel](https://bazel.build/) easier.

For the purpose of makeing develop easier, this project will try to work in macOS, Ubuntu and Fedora.

The principle of designing the structure is :

+ Shipshape - The code in this project is written under the rule [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html), prepare enough test case, and able to be used and tested ( or correctly omitted ) both in macOS and Linux
+ License - GPL is a good license, commercial licenses are very understandable, but we will meet some permission issues if we are writing code for private repository. In this case, currently this project only depends on the projects without risk in commercial usage
+ Interface - Helpers should be designed to call the libraries easily
+ Commonsensical - Most of the designing are tradeoff, the considering should be documented
+ Elastic - This repository itself should keep in lightweight, if we wish to load more resource, we should prepare the script lazily download from external storage instead of filled into current repository


## Usage

If you have already installed bazel (how to install? check it out [here](https://docs.bazel.build/versions/master/install.html)), and wish to run in local, please clone the project, and type 

```
bazel run //examples/cxx:hello_world
```

[![asciicast](https://asciinema.org/a/143587.png)](https://asciinema.org/a/143587)


You can copy examples to $PROJECT/app and run your code.

You can also 

```
docker pull yuikns/argcv
```

And run the code in your docker.

If you wish to try python in argcv (Who wish to take things so hard?), you should install numpy by yourself first, and then type

```
./setup
```

to make some configuration first.

You can also create a new repository and use it as a submodule. Please refer to [this repo](https://github.com/argcv/hello-argcv) for detail.


> TODO(yu): Some further introduction...

## Progress

### The Platform Itself

Currently, the project is in a very early stage. I am going to fill document now.


### Third-party Dependencies

This project is inspired by the design of [tensorflow](https://github.com/tensorflow/tensorflow).

1. The dependencies will NOT be downloaded until you are compiling the related code
2. The dependencies will be automatically downloaded, you are supposed to connect to the network in compiling

Here is a list of related libraries:

+ [x] [porter2_stemmer](https://github.com/smassung/porter2_stemmer) -- A implementation of <http://snowball.tartarus.org/algorithms/english/stemmer.html>
+ [x] [curl](https://curl.haxx.se/) -- a tool for talking to web servers
+ [x] [eigen](http://eigen.tuxfamily.org/) -- a C++ template library for linear algebra: vectors, matrices, and related algorithms
+ [x] [gflags](https://github.com/gflags/gflags) -- contains a C++ library that implements commandline flags processing
+ [x] [glog](https://github.com/google/glog) -- C++ implementation of the Google logging module
+ [x] [gtest](https://github.com/google/googletest) -- Google's C++ test framework
+ [x] [grpc](https://grpc.io/) -- A high performance, open-source universal RPC framework
+ [x] [leveldb](https://github.com/google/leveldb) -- a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values
+ [x] [libxml2](http://xmlsoft.org/) -- a XML C parser and toolkit
+ [ ] [libxsmm](https://github.com/hfp/libxsmm) -- Library targeting Intel Architecture for small, dense or sparse matrix multiplications, and small convolutions
+ [x] [nanopb](https://github.com/nanopb/nanopb) -- Protocol Buffers with small code size
+ [ ] [nasm](http://www.nasm.us/) -- a portable assembler, doc: <http://www.nasm.us/xdoc/2.13.01/html/nasmdoc0.html>
+ [x] [rapidjson](https://github.com/Tencent/rapidjson) -- A fast JSON parser/generator for C++ with both SAX/DOM style API
+ [x] [snappy](https://github.com/google/snappy) -- a fast compressor/decompressor
+ [ ] [zlib](https://zlib.net/) -- a massively spiffy yet delicately unobtrusive compression library
+ [ ] [tink](https://github.com/google/tink) -- a multi-language, cross-platform library that provides cryptographic APIs that are secure, easy to use correctly, and hard(er) to misuse.


##### For Python, will not fix soon

+ [ ] [numpy](https://github.com/numpy/numpy) -- fundamental package needed for scientific computing for python
+ [ ] Six  -- provides simple utilities for wrapping over differences between Python 2


[badge-gitter]:    https://badges.gitter.im/Join%20Chat.svg
[link-gitter]:     https://gitter.im/argcv/argcv?utm_source=share-link&utm_medium=link&utm_campaign=share-link
[badge-travis]:    https://travis-ci.org/yuikns/argcv.svg?branch=master
[link-travis]:     https://travis-ci.org/yuikns/argcv
[badge-license]:   https://img.shields.io/badge/license-MIT-007EC7.svg

[badge-status-bug]: https://img.shields.io/badge/status-bug-red.svg?style=flat
[badge-status-enhancement]: https://img.shields.io/badge/status-enhancement-blue.svg?style=flat
[badge-status-ready]: https://img.shields.io/badge/status-ready-green.svg?style=flat


## APIs

All the APIs are defined in `argcv/proto`, we didn't generate golang code in this repo. Please refer to [this repos](https://github.com/argcv/go-argcvapis) for the apis

