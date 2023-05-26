cmake_minimum_required(VERSION 3.0.0)
project(ctcp)
include_directories(
    include
)
add_library(lib_ctcp_server STATIC src/tcp_io.cpp src/tcp_server.cpp)
add_library(lib_ctcp_client STATIC src/tcp_io.cpp src/tcp_client.cpp)

add_executable(server_demo src/tcp_io.cpp src/tcp_server.cpp src/server_demo.cpp)
add_executable(client_demo src/tcp_io.cpp src/tcp_client.cpp src/client_demo.cpp)