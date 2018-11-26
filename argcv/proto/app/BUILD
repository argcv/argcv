package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "manul",
    deps = [
        ":manul_proto",
    ],
)

proto_library(
    name = "manul_proto",
    srcs = glob([
        "manul/*.proto",
    ]),
    deps = [
        # $ bazel query 'kind(proto_library, @com_google_protobuf//:all)'
        # @com_google_protobuf//:wrappers_proto
        # @com_google_protobuf//:timestamp_proto
        # @com_google_protobuf//:struct_proto
        # @com_google_protobuf//:field_mask_proto
        # @com_google_protobuf//:empty_proto
        # @com_google_protobuf//:duration_proto
        # @com_google_protobuf//:compiler_plugin_proto
        # @com_google_protobuf//:descriptor_proto
        # @com_google_protobuf//:api_proto
        # @com_google_protobuf//:type_proto
        # @com_google_protobuf//:source_context_proto
        # @com_google_protobuf//:any_proto
        "@com_google_protobuf//:any_proto",
        "@com_google_protobuf//:empty_proto",
        "@com_google_protobuf//:struct_proto",
        "//argcv/proto/api:annotations_proto",
        "//argcv/proto/status:status_proto",
    ],
)