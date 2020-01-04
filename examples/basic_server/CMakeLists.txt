cmake_minimum_required(VERSION 3.5)

project(basic_server)

add_executable(basic_server "basic_server.cpp")

target_include_directories(basic_server PUBLIC .)
target_link_libraries(basic_server PUBLIC server)
