cmake_minimum_required(VERSION 3.5)

project(PROJECT_C)

# This library does not have any source files
add_library(PROJECT_C INTERFACE)

set(BOOST_ROOT "~/binaries/boost-1.68")
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem)
find_package(Boost 1.68.0 EXACT REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(LIBCPP_INCLUDE "~/binaries/llvm-7.0.0/include/c++/v1")

set(INCLUDE_DIRS "${LIBCPP_INCLUDE}")
list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND INCLUDE_DIRS "${PROJECT_C_SOURCE_DIR}/include")

target_include_directories(PROJECT_C INTERFACE ${INCLUDE_DIRS})
target_compile_options(PROJECT_C INTERFACE -std=c++17 -stdlib=libc++)
target_link_libraries(PROJECT_C INTERFACE -nodefaultlibs ${Boost_LIBRARIES} -Wl,-Bstatic -lc++abi -lc++ -lunwind -Wl,-Bdynamic -lm -lc -ldl)

add_subdirectory(test)
