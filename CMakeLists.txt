cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(demo_gRPC)

add_subdirectory(proto)

# Set the main target name to the project name, can be changed if needed.
set(MAIN_TARGET_NAME ${PROJECT_NAME})

# Executable for the server
add_executable(${MAIN_TARGET_NAME}_server ${${PROJECT_NAME}_SOURCE_DIR}/server/server.cpp)

# Include directory (generated proto header files) for the server target
target_include_directories(${MAIN_TARGET_NAME}_server PUBLIC ${CMAKE_BINARY_DIR}/proto/myproto
                                                             ${${PROJECT_NAME}_SOURCE_DIR}/include)

# Link the server target to exported fw lib, proto lib
target_link_libraries(${MAIN_TARGET_NAME}_server proto_lib)

# Set the name of the binary file and the output directory
set_target_properties(${MAIN_TARGET_NAME}_server PROPERTIES OUTPUT_NAME "server")
set_target_properties(${MAIN_TARGET_NAME}_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_SOURCE_DIR}/bin)

# Shared library for client
add_library(${MAIN_TARGET_NAME}shared_lib_client ${${PROJECT_NAME}_SOURCE_DIR}/client/address_request.cpp
                                                 ${${PROJECT_NAME}_SOURCE_DIR}/client/value_request.cpp
                                                 ${${PROJECT_NAME}_SOURCE_DIR}/client/data_chunk_stream_request.cpp)

# Required directory for the shared library (generated proto headers will be added here too)
target_include_directories(${MAIN_TARGET_NAME}shared_lib_client PUBLIC ${CMAKE_BINARY_DIR}/proto/myproto
                                                                 ${${PROJECT_NAME}_SOURCE_DIR}/client)

target_link_libraries(${MAIN_TARGET_NAME}shared_lib_client proto_lib)

# Executable for the client
add_executable(${MAIN_TARGET_NAME}_client ${${PROJECT_NAME}_SOURCE_DIR}/client/client.cpp)

# Include directory for the executable target of client
target_include_directories(${MAIN_TARGET_NAME}_client PUBLIC ${${PROJECT_NAME}_SOURCE_DIR}/include)

# Link the executable target of client with the shared library
target_link_libraries(${MAIN_TARGET_NAME}_client ${MAIN_TARGET_NAME}shared_lib_client)

# Set the name of the binary file and the output directory
set_target_properties(${MAIN_TARGET_NAME}_client PROPERTIES OUTPUT_NAME "client")
set_target_properties(${MAIN_TARGET_NAME}_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_SOURCE_DIR}/bin)