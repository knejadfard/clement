cmake_minimum_required(VERSION 3.5)

project(server)

option(USE_LLVM "Use the LLVM toolchain" ON)
option(BUILD_DOC "Build documentation" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(CODE_COVERAGE "Generate code coverage report" OFF)

# This library does not have any source files
add_library(server INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem)
find_package(Threads REQUIRED)
find_package(Boost 1.67.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(CMAKE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ccov)

set(INCLUDE_DIRS "${LIBCPP_INCLUDE}")
list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND INCLUDE_DIRS "${server_SOURCE_DIR}/include")

target_include_directories(server INTERFACE ${INCLUDE_DIRS})

target_compile_options(server INTERFACE -std=c++17)
target_link_libraries(server INTERFACE Threads::Threads ${Boost_LIBRARIES})

if(USE_LLVM)
    target_compile_options(server INTERFACE -stdlib=libc++)
    target_link_libraries(server INTERFACE -nodefaultlibs -Wl,-Bstatic -lc++abi -lc++ -lunwind -llzma -Wl,-Bdynamic -lm -lc -ldl)
else(USE_LLVM)
    target_compile_options(server INTERFACE -static-libgcc)
    target_link_libraries(server INTERFACE -lm -lc -lgcc -lgcc_s)
endif(USE_LLVM)

add_subdirectory(test)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_DOC)
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Option for building documentation has been set to on but Doxygen was not found.")
    endif(NOT DOXYGEN_FOUND)
    if(NOT USE_LLVM)
        message(FATAL_ERROR "Currently building Doxygen documentation is only supported for LLVM.")
    endif(NOT USE_LLVM)
    set(LLVM_ROOT $ENV{LLVM_ROOT})
    set(DOXYGEN_INCLUDES "${server_SOURCE_DIR}/include")
    configure_file(doxygen.conf.in doc/doxygen.conf)
    add_custom_target(Doxygen ALL COMMAND Doxygen::doxygen doxygen.conf WORKING_DIRECTORY ./doc COMMENT "Building Doxygen documentation")
endif(BUILD_DOC)
