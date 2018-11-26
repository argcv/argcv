# How to prepare rules: https://bazel.build/versions/master/docs/skylark/cookbook.html
load("//tools/build_rules/config:system.bzl", "cc_system_package")
load("//tools/build_rules:cue.bzl", "cue")

def clean_dep(dep):
  return str(Label(dep))

# argcv workspace
def ws_argcv(repo_name="com_github_argcv_argcv"):
  # print("argcv configuring deps starting...")
  # cuda_configure(name = "local_config_cuda")
  # sycl_configure(name = "local_config_sycl")

  native.bind(
    name = "xsmm_avx",
    actual = "@libxsmm_archive//:xsmm_avx",
  )

  native.bind(
      name = "re2",
      actual = "@com_googlesource_code_re2//:re2",
  )

  native.bind(
      name = "nasm",
      actual = "@nasm_archive//:nasm",
  )

  # //external:gtest
  native.bind(
      name = "gtest",
      actual = "@gmock_archive//:gtest",
  )

  # //external:gtest_main
  native.bind(
      name = "gtest_main",
      actual = "@gmock_archive//:gtest_main",
  )

  # //external:snappy
  native.bind(
      name = "snappy",
      actual = "@snappy_archive//:snappy",
  )

  native.bind(
      name = "leveldb",
      actual = "@leveldb_archive//:leveldb",
  )

  # Bind of six
  native.bind(
      name = "six",
      actual = "@six_archive//:six",
  )

  # for python
  native.bind(
      name = "python_headers",
      actual = clean_dep("//util/python:python_headers"),
  )

  native.bind(
      name = "curl",
      actual = "@curl_archive//:curl",
  )

  # grpc expects //external:protobuf_clib and //external:protobuf_compiler
  # to point to the protobuf's compiler library.
  # note: used by grpc
  # note: update: use protobuf_compiler only
  native.bind(
      name = "protobuf_compiler",
      actual = "@com_google_protobuf//:protoc_lib",
  )

  native.bind(
      name = "protocol_compiler",
      actual = "@com_google_protobuf//:protoc",
  )

  native.bind(
      name = "protobuf_clib",
      actual = "@com_google_protobuf//:protoc_lib",
  )

  native.bind(
      name = "protoc",
      actual = "@com_google_protobuf//:protoc",
  )

  native.bind(
      name = "protobuf_headers",
      actual = "@com_google_protobuf//:protobuf_headers",
  )

  # required by grpc
  native.bind(
      name = "protobuf",
      actual = "@com_google_protobuf//:protobuf",
  )

  # gRPC wants a cares dependency but its contents is not actually
  # important if we have set GRPC_ARES=0 in tools/bazel.rc
  native.bind(
      name = "cares",
      actual = "@com_github_grpc_grpc//third_party/nanopb:nanopb",
  )

  native.bind(
      name = "nanopb",
      actual = "@com_github_grpc_grpc//third_party/nanopb:nanopb",
  )

  # protobuf expects //external:grpc_cpp_plugin to point to grpc's
  # C++ plugin code generator.
  native.bind(
      name = "grpc_cpp_plugin",
      actual = "@com_github_grpc_grpc//:grpc_cpp_plugin",
  )

  native.bind(
      name = "grpc_lib",
      actual = "@com_github_grpc_grpc//:grpc++_unsecure",
  )

  # GRPC C++ runtime library
  native.bind(
    name = "grpc++",
    actual = "@com_github_grpc_grpc//:grpc++"
  )

  native.bind(
    name = "grpc++_codegen_proto",
    actual = "@com_github_grpc_grpc//:grpc++_codegen_proto"
  )

  native.bind(
    name = "libssl",
    actual = "@boringssl//:ssl"
  )

  native.bind(
    name = "libssl_objc",
    actual = "@boringssl//:libssl_objc"
  )

  native.bind(
      name = "zlib",
      actual = "@zlib_archive//:zlib",
  )

  native.bind(
    name = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
  )

  native.bind(
    name = "glog",
    actual = "@com_github_google_glog//:glog",
  )

  native.bind(
    name = "benchmark",
    actual = "@com_github_google_benchmark//:benchmark",
  )

  native.bind(
    name = "benchmark_main",
    actual = "@com_github_google_benchmark//:benchmark_main",
  )

  native.bind(
    name = "rapidjson",
    actual = "@rapidjson_archive//:rapidjson",
  )

  native.bind(
    name = "linenoise",
    actual = "@linenoise_archive//:linenoise",
  )

  native.bind(
    name = "libxml2",
    actual = "@libxml2_archive//:libxml2",
  )

  native.bind(
    name = "yaml",
    actual = "@yaml_archive//:yaml",
  )

  native.bind(
    name = "cqueue",
    actual = "@concurrentqueue_archive//:concurrentqueue",
  )

  native.bind(
    name = "glm",
    actual = "@net_g_truc_glm//:glm",
  )


  # Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.
  # For more information go to http://eigen.tuxfamily.org/.
  cue(
    name = "eigen_archive",
    urls = [
        "https://mirror.bazel.build/bitbucket.org/eigen/eigen/get/6913f0cf7d06.tar.gz",
        "https://bitbucket.org/eigen/eigen/get/6913f0cf7d06.tar.gz",
    ],
    sha256 = "791b836cacd03e20bae5bdd25f1c4a5505a0a9975ba94a61eb4e2631fbd1d53a",
    strip_prefix = "eigen-eigen-6913f0cf7d06",
    build_file = clean_dep("//third_party:eigen.BUILD"),
    patch_file = clean_dep("//third_party:eigen_fix_cuda_compilation.patch"),
    repository = repo_name,
  )

  # ref: https://github.com/hfp/libxsmm
  # LIBXSMM is a library for small dense and small sparse matrix-matrix multiplications as well
  # as for deep learning primitives such as small convolutions targeting Intel Architecture.
  # Small matrix multiplication kernels are generated for the following instruction set extensions:
  # Intel SSE, Intel AVX, Intel AVX2, IMCI (KNCni) for Intel Xeon Phi coprocessors ("KNC"), and
  # Intel AVX‑512 as found in the Intel Xeon Phi processor family (Knights Landing "KNL", Knights
  # Mill "KNM") and Intel Xeon processors (Skylake-E "SKX"). Historically small matrix multiplications
  # were only optimized for the Intel Many Integrated Core Architecture "MIC") using intrinsic functions,
  # meanwhile optimized assembly code is targeting all afore mentioned instruction set extensions
  # (static code generation), and Just‑In‑Time (JIT) code generation is targeting Intel AVX and beyond.
  # Optimized code for small convolutions is JIT-generated for Intel AVX2 and Intel AVX‑512.
  # if "libxsmm_archive" not in native.existing_rules():
  cue(
      name = "libxsmm_archive",
      urls = [
          "https://mirror.bazel.build/github.com/hfp/libxsmm/archive/1.8.1.tar.gz",
          "https://github.com/hfp/libxsmm/archive/1.8.1.tar.gz",
      ],
      sha256 = "2ade869c3f42f23b5263c7d594aa3c7e5e61ac6a3afcaf5d6e42899d2a7986ce",
      strip_prefix = "libxsmm-1.8.1",
      build_file = clean_dep("//third_party:libxsmm.BUILD"),
      repository = repo_name,
  )

  # https://github.com/google/re2
  # RE2 is a fast, safe, thread-friendly alternative to backtracking regular expression engines
  # like those used in PCRE, Perl, and Python. It is a C++ library.
  cue(
      name = "com_googlesource_code_re2",
      urls = [
          "https://mirror.bazel.build/github.com/google/re2/archive/26cd968b735e227361c9703683266f01e5df7857.tar.gz",
          "https://github.com/google/re2/archive/26cd968b735e227361c9703683266f01e5df7857.tar.gz",

      ],
      sha256 = "e57eeb837ac40b5be37b2c6197438766e73343ffb32368efea793dfd8b28653b",
      strip_prefix = "re2-26cd968b735e227361c9703683266f01e5df7857",
      repository = repo_name,
  )

  cue(
      name = "nasm_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/www.nasm.us/pub/nasm/releasebuilds/2.12.02/nasm-2.12.02.tar.bz2",
          "http://pkgs.fedoraproject.org/repo/pkgs/nasm/nasm-2.12.02.tar.bz2/d15843c3fb7db39af80571ee27ec6fad/nasm-2.12.02.tar.bz2",
      ],
      sha256 = "00b0891c678c065446ca59bcee64719d0096d54d6886e6e472aeee2e170ae324",
      strip_prefix = "nasm-2.12.02",
      build_file = clean_dep("//third_party:nasm.BUILD"),
      repository = repo_name,
  )

  # gtest
  cue(
      name = "gmock_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/googletest/archive/release-1.8.0.zip",
          "https://github.com/google/googletest/archive/release-1.8.0.zip",
      ],
      sha256 = "f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
      strip_prefix = "googletest-release-1.8.0",
      build_file = clean_dep("//third_party:gmock.BUILD"),
      repository = repo_name,
  )

  # Description:
  # Snappy is a compression/decompression library 
  cue(
      name = "snappy_archive",
      urls = [
          "https://github.com/google/snappy/releases/download/1.1.4/snappy-1.1.4.tar.gz",
      ],
      sha256 = "134bfe122fd25599bb807bb8130e7ba6d9bdb851e0b16efcb83ac4f5d0b70057",
      strip_prefix = "snappy-1.1.4",
      build_file = clean_dep("//third_party:snappy.BUILD"),
      repository = repo_name,
  )

  # Description:
  # LevelDB is a fast key-value storage library written at Google that
  # provides an ordered mapping from string keys to string values.
  # native.new_http_archive(
  cue(
      name = "leveldb_archive",
      urls = [
          "https://github.com/google/leveldb/archive/v1.20.tar.gz",
      ],
      sha256 = "f5abe8b5b209c2f36560b75f32ce61412f39a2922f7045ae764a2c23335b6664",
      strip_prefix = "leveldb-1.20",
      build_file = clean_dep("//third_party:leveldb.BUILD"),
      repository = repo_name,
  )

  # Description:
  #   Six provides simple utilities for wrapping over differences between Python 2
  #   and Python 3.
  cue(
      name = "six_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/pypi.python.org/packages/source/s/six/six-1.10.0.tar.gz",
          "http://pypi.python.org/packages/source/s/six/six-1.10.0.tar.gz",
      ],
      sha256 = "105f8d68616f8248e24bf0e9372ef04d3cc10104f1980f54d57b2ce73a5ad56a",
      strip_prefix = "six-1.10.0",
      build_file = clean_dep("//third_party:six.BUILD"),
      repository = repo_name,
  )

  # curl
  cue(
      name = "curl_archive",
      sha256 = "ff3e80c1ca6a068428726cd7dd19037a47cc538ce58ef61c59587191039b2ca6",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/curl.haxx.se/download/curl-7.49.1.tar.gz",
          "https://curl.haxx.se/download/curl-7.49.1.tar.gz",
      ],
      strip_prefix = "curl-7.49.1",
      build_file = clean_dep("//third_party:curl.BUILD"),
      repository = repo_name,
  )

  # It is requreid by grpc
  # We need to import the protobuf library under the names com_google_protobuf
  # and com_google_protobuf_cc to enable proto_library support in bazel.
  # Unfortunately there is no way to alias http_archives at the moment.
  # update note: I have already removed related deps
  cue(
      name = "com_google_protobuf",
      urls = [
          "https://github.com/google/protobuf/archive/v3.6.0.1.tar.gz"
      ],
      sha256 = "1144ef1fa9c50d3c04880f363b988df6ca6a66e337a945f1661cf4256ffba749",
      strip_prefix = "protobuf-3.6.0.1",
      repository = repo_name,
  )

  # java_lite_proto_library rules implicitly depend on @com_google_protobuf_javalite//:javalite_toolchain,
  # which is the JavaLite proto runtime (base classes and common utilities).
  # ref: https://github.com/cgrushko/proto_library
  cue(
      name = "com_google_protobuf_javalite",
      sha256 = "d8a2fed3708781196f92e1e7e7e713cf66804bd2944894401057214aff4f468e",
      strip_prefix = "protobuf-5e8916e881c573c5d83980197a6f783c132d4276",
      urls = ["https://github.com/google/protobuf/archive/5e8916e881c573c5d83980197a6f783c132d4276.zip"],
      repository = repo_name,
  )

  # TODO protobuf_java, protobuf_java_util etc.
  cue(
      name = "com_github_grpc_grpc",
      urls = [
          "https://mirror.bazel.build/github.com/grpc/grpc/archive/730b778632e79cc3c96ad237f282d687ee325ce7.tar.gz",
          "https://github.com/grpc/grpc/archive/730b778632e79cc3c96ad237f282d687ee325ce7.tar.gz",
      ],
      sha256 = "8c91a8d12e1e868cf51f7340b75507a8aa017a7e1b56f46ed6816aeb803dc9bd",
      strip_prefix = "grpc-730b778632e79cc3c96ad237f282d687ee325ce7",
      repository = repo_name,
  )


  # branch: master-with-bazel
  cue(
      name = "boringssl",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/github.com/google/boringssl/archive/5ddc9cd665162147eeea3283cc8253f61bc932cf.tar.gz",
          "https://github.com/google/boringssl/archive/5ddc9cd665162147eeea3283cc8253f61bc932cf.tar.gz",  # 2017-09-12
      ],
      sha256 = "4a39e013759b5d6cdaad7f1f19361835cec34c984c3faaec0180a67079936445",
      strip_prefix = "boringssl-5ddc9cd665162147eeea3283cc8253f61bc932cf",
      repository = repo_name,
  )


  cue(
      name = "com_google_absl",
      urls = [
          "https://mirror.bazel.build/github.com/abseil/abseil-cpp/archive/720c017e30339fd1786ce4aac68bc8559736e53f.tar.gz",
          "https://github.com/abseil/abseil-cpp/archive/720c017e30339fd1786ce4aac68bc8559736e53f.tar.gz",
      ],
     sha256 = "5996380e3e8b981f55d1c8d58e709c00dbb4806ba367be75d0925a68cc2f6478",
     strip_prefix = "abseil-cpp-720c017e30339fd1786ce4aac68bc8559736e53f",
     build_file = clean_dep("//third_party:com_google_absl.BUILD"),
     repository = repo_name,
  )

  # zlib is designed to be a free, general-purpose, legally unencumbered -- that is, not covered by any patents 
  # -- lossless data-compression library for use on virtually any computer hardware and operating system. 
  # The zlib data format is itself portable across platforms. Unlike the LZW compression method used in Unix compress(1) 
  # and in the GIF image format, the compression method currently used in zlib essentially never expands the data
  #. (LZW can double or triple the file size in extreme cases.) zlib's memory footprint is also independent of the input 
  # data and can be reduced, if necessary, at some cost in compression. A more precise, technical discussion 
  # of both points is available on another page.
  cue(
      name = "zlib_archive",
      urls = [
          "http://bazel-mirror.storage.googleapis.com/zlib.net/zlib-1.2.8.tar.gz",
          "http://zlib.net/fossils/zlib-1.2.8.tar.gz",
      ],
      sha256 = "36658cb768a54c1d4dec43c3116c27ed893e88b02ecfcb44f2166f9c0b7f2a0d",
      strip_prefix = "zlib-1.2.8",
      build_file = clean_dep("//third_party:zlib.BUILD"),
      repository = repo_name,
  )

  cue(
      name = "com_github_gflags_gflags",
      urls = [
          "https://github.com/gflags/gflags/archive/v2.2.0.zip",
      ],
      sha256 = "99f9e8b63ea53e4b23f0fd0f68a6f1e397d5512be36716c17cc75966a90f0d57",
      strip_prefix = "gflags-2.2.0",
      build_file = clean_dep("//third_party:gflags.BUILD"), # Use Customized Build File
      repository = repo_name,
  )

  cue(
    name = "com_github_google_glog",
    urls = [
      "https://github.com/google/glog/archive/da816ea70645e463aa04f9564544939fa327d5a7.zip",
    ],
    build_file = clean_dep("//third_party:glog.BUILD"),
    sha256 = "b2f35994772423a6f51e885e57a345350bac70a5aee8e035428643c0a0a4ca30",
    strip_prefix = "glog-da816ea70645e463aa04f9564544939fa327d5a7",
    repository = repo_name,
  )

  cue(
    name = "com_github_google_benchmark",
    urls = [
      "https://github.com/google/benchmark/archive/4c2af0788977d3bd900585528c2d08b875b2cd39.zip",
    ],
    # build_file = clean_dep(""),
    sha256 = "d1ab75f9e20c294246a31ff25035c0239d7367804edbf6529ae98eed1f79ddf7",
    strip_prefix = "benchmark-4c2af0788977d3bd900585528c2d08b875b2cd39",
    repository = repo_name,
  )

  cue(
      name = "rapidjson_archive",
      urls = [
          "https://github.com/miloyip/rapidjson/archive/v1.1.0.tar.gz",
      ],
      sha256 = "bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98e",
      strip_prefix = "rapidjson-1.1.0",
      build_file = clean_dep("//third_party:rapidjson.BUILD"),
      repository = repo_name,
  )

  # TODO(yu): rewrite for unicode issue and c++
  cue(
      name = "linenoise_archive",
      urls = [
          "https://github.com/yuikns/linenoise/archive/2105ce445821381cf1bca87b6d386d4ea88ee20d.tar.gz",
      ],
      sha256 = "06a9727c8623f434d686f3782988a55d92e7da759fe14594a6eaa6b13764e8b9",
      strip_prefix = "linenoise-2105ce445821381cf1bca87b6d386d4ea88ee20d",
      build_file = clean_dep("//third_party:linenoise.BUILD"),
      repository = repo_name,
  )

  cue(
    name = "libxml2_archive",
    urls = [
        "http://xmlsoft.org/sources/libxml2-2.9.6.tar.gz",
    ],
    sha256 = "8b9038cca7240e881d462ea391882092dfdc6d4f483f72683e817be08df5ebbc",
    strip_prefix = "libxml2-2.9.6",
    build_file = clean_dep("//third_party:libxml2.BUILD"),
    repository = repo_name,
  )
  
  cue(
      name = "yaml_archive",
      urls = [
          "https://github.com/jbeder/yaml-cpp/archive/5d5bb52ec2825eb4c0c932a9c2d50dc3699f932f.tar.gz",
      ],
      sha256 = "81b4f1d0a5a1f47b3cff818bac9952079042bebe667126ec9972f81320be69b1",
      strip_prefix = "yaml-cpp-5d5bb52ec2825eb4c0c932a9c2d50dc3699f932f",
      build_file = clean_dep("//third_party:yaml.BUILD"),
      repository = repo_name,
  )

  cue(
      name = "concurrentqueue_archive",
      urls = [
          "https://github.com/cameron314/concurrentqueue/archive/8f7e861dd9411a0bf77a6b9de83a47b3424fafba.tar.gz",
      ],
      sha256 = "7f656c7f3c6f07fe23e1794d853af09624a640bfc34823dba189998da4cb085b",
      strip_prefix = "concurrentqueue-8f7e861dd9411a0bf77a6b9de83a47b3424fafba",
      build_file = clean_dep("//third_party:concurrentqueue.BUILD"),
      delete = [
          "build", # remove conflict file
      ],
      repository = repo_name,
  )

  cue(
      name = "net_g_truc_glm",
      urls = [
          "https://github.com/g-truc/glm/archive/620c951501b4c7da08565940a09a9139cfb6353c.tar.gz",
      ],
      sha256 = "1fed6fd55d066aeb028156604678c4e8cfa503db2fa99e7023b5b70ff6cb2c59",
      strip_prefix = "glm-620c951501b4c7da08565940a09a9139cfb6353c",
      build_file = clean_dep("//third_party:glm.BUILD"),
      repository = repo_name,
  )

  # System Dependencies

  cc_system_package(
    name = "libmongocxx",
    modname = "libmongocxx",
  )

  cc_system_package(
    name = "fuse",
    modname = "fuse",
  )
  
  cc_system_package(
    name = "tf",
    modname = "tensorflow",
  )

  cc_system_package(
    name = "glfw3",
    modname = "glfw3",
  )

  cc_system_package(
    name = "freetype2",
    modname = "freetype2",
  )

# # print("argcv configuring deps finished...")
