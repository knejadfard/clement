cmake_minimum_required(VERSION 3.5)

project(server_test)
enable_testing()

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
foreach(TEST_SRC ${TEST_SRCS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SRC})
    target_include_directories(${TEST_NAME} PRIVATE ${Boost_INCLUDE_DIRS})

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(${TEST_NAME} PRIVATE -std=c++17 -stdlib=libc++)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${TEST_NAME} PRIVATE -std=c++17)
    endif()

    target_link_libraries(${TEST_NAME} PRIVATE clement)
    target_link_libraries(${TEST_NAME} PRIVATE Catch2)
    add_subdirectory(Catch2)

    if(CODE_COVERAGE)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            target_compile_options(${TEST_NAME} PRIVATE -fno-use-cxa-atexit -fprofile-instr-generate -fcoverage-mapping)
            target_link_libraries(${TEST_NAME} PRIVATE -fprofile-instr-generate)

            add_custom_target(generate-prof-${TEST_NAME}
                COMMAND rm -f ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                COMMAND rm -f ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}

                COMMAND LLVM_PROFILE_FILE=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profraw $<TARGET_FILE:${TEST_NAME}>
                COMMAND echo "-object=$<TARGET_FILE:${TEST_NAME}>" >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/binaries.list
                COMMAND echo "${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profraw " >> ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/profraw.list

                COMMAND llvm-profdata merge -sparse ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profraw -o ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profdata
            )

            add_custom_target(ccov-show-${TEST_NAME}
                COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}> -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profdata -show-line-counts-or-regions
                DEPENDS generate-prof-${TEST_NAME})

            add_custom_target(ccov-report-${TEST_NAME}
                COMMAND llvm-cov report $<TARGET_FILE:${TEST_NAME}> -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profdata "${CMAKE_SOURCE_DIR}/include"
                DEPENDS generate-prof-${TEST_NAME})

            add_custom_target(ccov-html-${TEST_NAME}
                COMMAND llvm-cov show $<TARGET_FILE:${TEST_NAME}>
                      -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profdata
                      -show-line-counts-or-regions
                      -output-dir=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}
                      -format="html"
                      "${CMAKE_SOURCE_DIR}/include"
                DEPENDS generate-prof-${TEST_NAME})

            add_custom_target(ccov-lcov-${TEST_NAME}
                COMMAND llvm-cov export -format=lcov -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.profdata $<TARGET_FILE:${TEST_NAME}> "${CMAKE_SOURCE_DIR}/include" > ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/${TEST_NAME}.lcov
                DEPENDS generate-prof-${TEST_NAME})
        else()
            target_compile_options(${TEST_NAME} PRIVATE -fno-use-cxa-atexit -fprofile-arcs -ftest-coverage)
            target_link_libraries(${TEST_NAME} PRIVATE -fprofile-arcs)
        endif()
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_SRC)
