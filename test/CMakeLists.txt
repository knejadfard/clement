cmake_minimum_required(VERSION 3.5)

project(PROJECT_C_test)
enable_testing()

# If Boost is placed in a non-standard location,
# this property needs to be set to be able to
# find the location of relevant Boost files.
set(BOOST_ROOT "~/compiled/boost_1.67.0")
set(Boost_USE_STATIC_LIBS ON)
set(BOOST_COMPONENTS thread system log_setup log filesystem unit_test_framework)
find_package(Boost 1.67.0 EXACT REQUIRED COMPONENTS ${BOOST_COMPONENTS})

set(LIBCPP_INCLUDE "~/compiled/llvm/include/c++/v1")

set(INCLUDE_DIRS "${LIBCPP_INCLUDE}")
list(APPEND INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
list(APPEND INCLUDE_DIRS "${PROJECT_C_SOURCE_DIR}/include")

set(LIBS -lc++ -lc++abi)
list(APPEND LIBS -lc -lm -lgcc_s)
list(APPEND LIBS -lpthread)
list(APPEND LIBS ${Boost_LIBRARIES})

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${INCLUDE_DIRS})
    target_compile_options(${TEST_NAME} PRIVATE -std=c++17 -stdlib=libc++ -Wall -fprofile-instr-generate -fcoverage-mapping -fprofile-arcs -ftest-coverage)
    target_link_libraries(${TEST_NAME} PRIVATE ${LIBS} -fprofile-arcs -ftest-coverage)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_SRC)
