find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Threads)

#
# Protobuf/Grpc source files
#
set(PROTO_FILES
    ${${PROJECT_NAME}_SOURCE_DIR}/proto/myproto/big_data.proto
    ${${PROJECT_NAME}_SOURCE_DIR}/proto/myproto/addressbook.proto
    ${${PROJECT_NAME}_SOURCE_DIR}/proto/myproto/collection.proto
)

#
# Add Library target with protobuf sources
#
message($PROTO_CMAKE_${PROJECT_SOURCE_DIR})
add_library(proto_lib ${PROTO_FILES})
target_link_libraries(proto_lib
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)

# Include directory of the generated proto header against the target proto_lib
# Directory is: ${${PROJECT_NAME}_SOURCE_DIR}/build/proto/proto_files
message($PROTO_CMAKE_CMAKE_CURRENT_BINARY_DIR_${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(proto_lib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

#
# Compile protobuf and grpc files in proto_lib target to cpp
#
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET proto_lib LANGUAGE cpp)
protobuf_generate(TARGET proto_lib LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
