cmake_minimum_required(VERSION 3.5)

project(PROJECT_C)

option(BUILD_EXAMPLES "Build examples" ON)
option(USE_LLVM "Use the LLVM toolchain" ON)

# This library does not have any source files
add_library(PROJECT_C INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem)
find_package(Boost 1.68.0 EXACT REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(INCLUDE_DIRS "${LIBCPP_INCLUDE}")
list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND INCLUDE_DIRS "${PROJECT_C_SOURCE_DIR}/include")

target_include_directories(PROJECT_C INTERFACE ${INCLUDE_DIRS})

target_compile_options(PROJECT_C INTERFACE -std=c++17)
target_link_libraries(PROJECT_C INTERFACE ${Boost_LIBRARIES})

if(USE_LLVM)
    target_compile_options(PROJECT_C INTERFACE -stdlib=libc++)
    target_link_libraries(PROJECT_C INTERFACE -nodefaultlibs -Wl,-Bstatic -lc++abi -lc++ -lunwind -Wl,-Bdynamic -lm -lc -ldl)
else(USE_LLVM)
    target_compile_options(PROJECT_C INTERFACE -static-libgcc -static-libstdc++)
    target_link_libraries(PROJECT_C INTERFACE -lm -lc -lgcc -lgcc_s)
endif(USE_LLVM)

add_subdirectory(test)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)
