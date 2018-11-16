cmake_minimum_required(VERSION 3.5)

project(PROJECT_C)

# This library does not have any source files
add_library(PROJECT_C INTERFACE)

set(INCLUDE_DIRS "include" "/home/kian/compiled/llvm/include/c++/v1")

target_include_directories(PROJECT_C INTERFACE ${INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
target_compile_options(PROJECT_C INTERFACE -std=c++17 -stdlib=libc++)
target_link_libraries(PROJECT_C INTERFACE -nodefaultlibs -lc++ -lc++abi -lc -lm -lgcc_s -lgcc ${Boost_LIBRARIES} -lpthread)

add_subdirectory(test)
