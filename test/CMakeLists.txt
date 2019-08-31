cmake_minimum_required(VERSION 3.5)

project(PROJECT_C_test)
enable_testing()

find_package(Boost 1.67.0 REQUIRED COMPONENTS unit_test_framework)

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

    target_compile_options(${TEST_NAME} PRIVATE -std=c++17 -stdlib=libc++)
    target_link_libraries(${TEST_NAME} PRIVATE PROJECT_C)

    if(CODE_COVERAGE)
        target_compile_options(${TEST_NAME} PRIVATE -fno-use-cxa-atexit -fprofile-instr-generate -fcoverage-mapping -fprofile-arcs -ftest-coverage)
        target_link_libraries(${TEST_NAME} PRIVATE -fprofile-arcs -ftest-coverage)

        add_custom_target(ccov-clean
            COMMAND rm
            -f
            ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
            COMMAND rm
            -f
            ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list)

        add_custom_target(ccov-preprocessing
            COMMAND ${CMAKE_COMMAND}
            -E
            make_directory ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}
            DEPENDS ccov-clean)

        add_custom_target(ccov-run-${TEST_NAME}
            COMMAND LLVM_PROFILE_FILE=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profraw $<TARGET_FILE:${TEST_NAME}>
            COMMAND echo "-object=$<TARGET_FILE:${TEST_NAME}>" >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
            COMMAND echo "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profraw " >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list
            DEPENDS ccov-preprocessing ${TEST_NAME})

        add_custom_target(ccov-processing-${TEST_NAME}
            COMMAND ${LLVM_ROOT}/bin/llvm-profdata merge -sparse ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profraw -o ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profdata
            DEPENDS ccov-run-${TEST_NAME})

        add_custom_target(ccov-show-${TEST_NAME}
            COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}> -instr-profile=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profdata -show-line-counts-or-regions
            DEPENDS ccov-processing-${TEST_NAME})

        add_custom_target(ccov-report-${TEST_NAME}
            COMMAND llvm-cov report $<TARGET_FILE:${TEST_NAME}> -instr-profile=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profdata
            DEPENDS ccov-processing-${TEST_NAME})

        add_custom_target(ccov-${TEST_NAME}
            COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}>
                  -instr-profile=${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.profdata
                  -show-line-counts-or-regions
                  -output-dir=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}
                  -format="html"
                  ${EXCLUDE_REGEX}
            DEPENDS ccov-processing-${TEST_NAME})
        add_custom_command(
            TARGET ccov-${TEST_NAME} POST_BUILD
            COMMAND ;
            COMMENT "Open ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}/index.html in your browser to view the coverage report.")
    endif()
endforeach(TEST_SRC)
