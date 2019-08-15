cmake_minimum_required(VERSION 3.5)

project(PROJECT_C)

option(BUILD_DOC "Build documentation" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(USE_LLVM "Use the LLVM toolchain" ON)

# This library does not have any source files
add_library(PROJECT_C INTERFACE)

set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem)
find_package(Boost 1.67.0 REQUIRED COMPONENTS ${BOOST_COMPONENTS})

if(BUILD_DOC)
    find_package(Doxygen)
endif(BUILD_DOC)

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

if(BUILD_DOC)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Option for building documentation has been set to on but Doxygen was not found.")
    endif(NOT DOXYGEN_FOUND)
    if(NOT USE_LLVM)
        message(FATAL_ERROR "Currently building Doxygen documentation is only supported for LLVM.")
    endif(NOT USE_LLVM)
    set(LLVM_ROOT $ENV{LLVM_ROOT})
    set(DOXYGEN_INCLUDES "${PROJECT_C_SOURCE_DIR}/include")
    configure_file(doxygen.conf.in doc/doxygen.conf)
    add_custom_target(Doxygen ALL COMMAND Doxygen::doxygen doxygen.conf WORKING_DIRECTORY ./doc COMMENT "Building Doxygen documentation")
endif(BUILD_DOC)
