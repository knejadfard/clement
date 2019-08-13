cmake_minimum_required(VERSION 3.5)

project(PROJECT_C_test)
enable_testing()

find_package(Boost 1.67.0 REQUIRED COMPONENTS unit_test_framework)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${TEST_NAME} PRIVATE PROJECT_C ${Boost_LIBRARIES})

    # Code coverage stuff
    target_compile_options(${TEST_NAME} PRIVATE -fno-use-cxa-atexit -fprofile-instr-generate -fcoverage-mapping -fprofile-arcs -ftest-coverage)
    target_link_libraries(${TEST_NAME} PRIVATE lzma -fprofile-arcs -ftest-coverage)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    # llvm-cov
add_custom_target(${TEST_NAME}-ccov-preprocessing
    COMMAND LLVM_PROFILE_FILE=${TEST_NAME}.profraw $<TARGET_FILE:${TEST_NAME}>
    COMMAND llvm-profdata merge -sparse ${TEST_NAME}.profraw -o ${TEST_NAME}.profdata
    DEPENDS ${TEST_NAME})

add_custom_target(${TEST_NAME}-ccov-show
    COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}> -instr-profile=${TEST_NAME}.profdata -show-line-counts-or-regions ${CMAKE_SOURCE_DIR}/include
    DEPENDS ${TEST_NAME}-ccov-preprocessing)

add_custom_target(${TEST_NAME}-ccov-report
    COMMAND llvm-cov report $<TARGET_FILE:${TEST_NAME}> -instr-profile=${TEST_NAME}.profdata ${CMAKE_SOURCE_DIR}/include
    DEPENDS ${TEST_NAME}-ccov-preprocessing)

add_custom_target(${TEST_NAME}-ccov
    COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}> -instr-profile=${TEST_NAME}.profdata -show-line-counts-or-regions -output-dir=${TEST_NAME}-llvm-cov -format="html" ${CMAKE_SOURCE_DIR}/include
    DEPENDS ${TEST_NAME}-ccov-preprocessing)

add_custom_command(TARGET ${TEST_NAME}-ccov POST_BUILD
    COMMAND ;
    COMMENT "Open ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME}-llvm-cov/index.html in your browser to view the coverage report."
)
endforeach(TEST_SRC)
