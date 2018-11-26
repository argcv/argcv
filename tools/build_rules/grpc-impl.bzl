# Convert proto definition to C++/Java code
# based on https://github.com/google/kythe/blob/master/tools/build_rules/proto.bzl
# from https://github.com/mzhaom/trunk/blob/master/tools/build_rules/grpc/grpc_proto.bzl
# currently, it supports cxx only
# last wip for java and python: https://github.com/yuikns/argcv/blob/85d3bde62ca90279489eeb4012b098ad734ecd96/bazel/grpc_proto.bzl
# TODO(yu): sync with API in https://github.com/grpc/grpc/blob/v1.10.0-pre2/bazel/cc_grpc_library.bzl
# TODO(yu): reuse native.proto_library

def collect_proto_sources(ctx):
  source_files = depset(order="postorder")
  for dep in ctx.attr.deps:
    source_files += dep.proto_src
  source_files += [ctx.file.src]
  return source_files

def _genproto_impl(ctx):
  proto_srcs = collect_proto_sources(ctx)
  inputs, outputs, arguments = list(proto_srcs), [], []
  if ctx.attr.gen_cc:
    outputs += [ctx.outputs.cc_hdr, ctx.outputs.cc_src]
    arguments += ["--cpp_out=" + ctx.configuration.genfiles_dir.path]
    if ctx.attr.has_service:
      grpc_cpp_plugin = ctx.executable.grpc_cpp_plugin
      inputs += [grpc_cpp_plugin]
      outputs += [ctx.outputs.cc_grpc_hdr, ctx.outputs.cc_grpc_src]
      arguments += [
        "--grpc_out=" + ctx.configuration.genfiles_dir.path,
        "--plugin=protoc-gen-grpc=" + grpc_cpp_plugin.path,
      ]

  ctx.action(
      mnemonic = "GenProto",
      inputs = inputs,
      outputs = outputs,
      arguments = arguments + [ctx.file.src.path],
      executable = ctx.executable._protoc)

  return struct(files=depset(outputs),
                proto_src=proto_srcs)

_genproto_attrs = {
    "src": attr.label(
        allow_files = FileType([".proto"]),
        mandatory = True,
        single_file = True,
    ),
    "deps": attr.label_list(
        allow_files = False,
        providers = ["proto_src"],
    ),
    "has_service": attr.bool(),
    "_protoc": attr.label(
        default = Label("//external:protoc"),
        cfg = "host",
        executable = True,
    ),
    "grpc_cpp_plugin": attr.label(
        # We don't specify the default here, otherwise it imposes an
        # implicit dependency to the plugin, which we may not need
        # depending on when gen_cc is true.
        # default = Label("//external:grpc_cpp_plugin"),
        cfg = "host",
        executable = True,
    ),
    "gen_cc": attr.bool(),
}

def _genproto_outputs(gen_cc, has_service):
  outputs = {}
  if gen_cc:
    outputs += {
      "cc_hdr": "%{src}.pb.h",
      "cc_src": "%{src}.pb.cc"
    }
    if has_service:
      outputs += {
        "cc_grpc_hdr": "%{src}.grpc.pb.h",
        "cc_grpc_src": "%{src}.grpc.pb.cc"
      }
  return outputs

genproto = rule(
    _genproto_impl,
    attrs = _genproto_attrs,
    output_to_genfiles = True,
    outputs = _genproto_outputs,
)

def _invoke(rulefn, name, **kwargs):
  """Invoke rulefn with name and kwargs, returning the label of the rule."""
  if hasattr(native, "package_name"):
    rulefn(name=name, **kwargs)
    return "//{}:{}".format(native.package_name(), name)
  else:
    return rulefn(name=name, **kwargs).label()
# return "//{}:{}".format(native.package_name(), name)

def cc_grpc_library(name, src, deps=[],
                  visibility=None,
                  generate_cc=True,
                  has_service=False):
  genproto_deps = [dep + "_genproto" for dep in deps]
  if generate_cc and has_service:
    grpc_cpp_plugin = "//external:grpc_cpp_plugin"
  else:
    grpc_cpp_plugin = None

  proto_pkg = _invoke(genproto,
    name=name + "_genproto",
    visibility = visibility,
    src=src,
    deps=genproto_deps,
    has_service=has_service,
    gen_cc=generate_cc,
    grpc_cpp_plugin = grpc_cpp_plugin)
    
  print("[" + str(proto_pkg) + "]")

  if generate_cc:
    cc_deps = ["//external:protobuf_compiler"]
    if has_service:
      cc_deps += ["//external:grpc++"]
    cc_deps += deps
    native.cc_library(
      name = name,
      visibility = visibility,
      hdrs = [proto_pkg],
      srcs = [proto_pkg],
      defines = ["GOOGLE_PROTOBUF_NO_RTTI"],
      deps = cc_deps,
    )
