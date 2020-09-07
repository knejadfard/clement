cmake_minimum_required(VERSION 3.10)

project(Catch2)
add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
