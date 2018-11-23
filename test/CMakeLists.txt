cmake_minimum_required(VERSION 3.5)

project(PROJECT_C_test)
enable_testing()

find_package(Boost 1.68.0 EXACT REQUIRED COMPONENTS unit_test_framework)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${TEST_NAME} PRIVATE PROJECT_C ${Boost_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_SRC)
