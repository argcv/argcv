package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "annotations",
    deps = [
      ":annotations_proto",
    ],
)

cc_proto_library(
    name = "http",
    deps = [
      ":http_proto",
    ],
)

cc_proto_library(
    name = "httpbody",
    deps = [
      ":httpbody_proto",
    ],
)

cc_proto_library(
    name = "label",
    deps = [
      ":label_proto",
    ],
)

proto_library(
    name = "annotations_proto",
    srcs = [
      "annotations.proto",
    ],
    deps = [
        # $ bazel query 'kind(proto_library, @com_google_protobuf//:all)'
        ":http_proto",
        "@com_google_protobuf//:descriptor_proto",
    ],
)

proto_library(
    name = "http_proto",
    srcs = [
      "http.proto",
    ],
)

proto_library(
    name = "httpbody_proto",
    srcs = [
      "httpbody.proto",
    ],
)

proto_library(
    name = "label_proto",
    srcs = [
      "label.proto",
    ],
)


