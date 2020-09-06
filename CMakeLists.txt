cmake_minimum_required(VERSION 3.5)

project(clement)

option(BUILD_DOC "Build documentation" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(CODE_COVERAGE "Generate code coverage report" OFF)

# This library does not have any source files
add_library(clement INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem)
find_package(Threads REQUIRED)
find_package(Boost 1.67.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(CMAKE_COVERAGE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ccov)

set(INCLUDE_DIRS "${LIBCPP_INCLUDE}")
list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND INCLUDE_DIRS "${clement_SOURCE_DIR}/include")

target_include_directories(clement INTERFACE ${INCLUDE_DIRS})

target_compile_options(clement INTERFACE -std=c++17)
target_link_libraries(clement INTERFACE Threads::Threads ${Boost_LIBRARIES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(clement INTERFACE -stdlib=libc++)
    target_link_libraries(clement INTERFACE -nodefaultlibs -Wl,-Bstatic -lc++abi -lc++ -lunwind -llzma -Wl,-Bdynamic -lm -lc -ldl)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(clement INTERFACE -static-libgcc)
    target_link_libraries(clement INTERFACE -lm -lc -lgcc -lgcc_s)
endif()

add_subdirectory(test)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(BUILD_DOC)
    find_package(Doxygen)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Option for building documentation has been set to on but Doxygen was not found.")
    endif()
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "Currently building Doxygen documentation is only supported for LLVM.")
    endif()
    set(LLVM_ROOT $ENV{LLVM_ROOT})
    set(DOXYGEN_INCLUDES "${clement_SOURCE_DIR}/include")
    configure_file(doxygen.conf.in doc/doxygen.conf)
    add_custom_target(Doxygen ALL COMMAND Doxygen::doxygen doxygen.conf WORKING_DIRECTORY ./doc COMMENT "Building Doxygen documentation")
endif()
