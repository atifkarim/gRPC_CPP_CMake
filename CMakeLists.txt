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
set_target_properties(${MAIN_TARGET_NAME}_server PROPERTIES OUTPUT_NAME "server_executable")
set_target_properties(${MAIN_TARGET_NAME}_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_SOURCE_DIR}/bin)

# Executable for the server
add_executable(${MAIN_TARGET_NAME}_client ${${PROJECT_NAME}_SOURCE_DIR}/client/client.cpp)

# Include directory (generated proto header files) for the client target
target_include_directories(${MAIN_TARGET_NAME}_client PUBLIC ${CMAKE_BINARY_DIR}/proto/myproto
                                                             ${${PROJECT_NAME}_SOURCE_DIR}/include)

# Link the client target to proto lib
target_link_libraries(${MAIN_TARGET_NAME}_client proto_lib)

# Set the name of the binary file and the output directory
set_target_properties(${MAIN_TARGET_NAME}_client PROPERTIES OUTPUT_NAME "client_executable")
set_target_properties(${MAIN_TARGET_NAME}_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${PROJECT_NAME}_SOURCE_DIR}/bin)