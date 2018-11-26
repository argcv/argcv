package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "error_codes",
    deps = [
      ":error_codes_proto",
    ],
)

cc_proto_library(
    name = "error_details",
    deps = [
      ":error_details_proto",
    ],
)

cc_proto_library(
    name = "status",
    deps = [
      ":status_proto",
    ],
)

proto_library(
    name = "error_codes_proto",
    srcs = [
      "error_codes.proto",
    ],
)

proto_library(
    name = "error_details_proto",
    srcs = [
      "error_details.proto",
    ],
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
        "@com_google_protobuf//:duration_proto",
    ],
)

proto_library(
    name = "status_proto",
    srcs = [
      "status.proto",
    ],
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
        ":error_codes_proto",
        "@com_google_protobuf//:any_proto",
    ],
)
