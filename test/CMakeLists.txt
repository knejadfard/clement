cmake_minimum_required(VERSION 3.5)

project(unit_tests)
enable_testing()

file(GLOB TEST_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
add_executable(unit_tests ${TEST_SRCS})

target_include_directories(unit_tests PRIVATE ${Boost_INCLUDE_DIRS})

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(unit_tests PRIVATE -std=c++17 -stdlib=libc++)
    if(CODE_COVERAGE)
        target_compile_options(unit_tests PRIVATE -fno-use-cxa-atexit -fprofile-instr-generate -fcoverage-mapping)
        target_link_libraries(unit_tests PRIVATE -fprofile-instr-generate)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(unit_tests PRIVATE -std=c++17)
    if(CODE_COVERAGE)
        target_compile_options(unit_tests PRIVATE -fno-use-cxa-atexit -fprofile-arcs -ftest-coverage)
        target_link_libraries(unit_tests PRIVATE -fprofile-arcs)
    endif()
endif()

target_link_libraries(unit_tests PRIVATE clement)
target_link_libraries(unit_tests PRIVATE Catch2)
add_subdirectory(Catch2)

add_test(NAME unit_tests COMMAND unit_tests)

add_custom_target(generate-prof
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}
    COMMAND LLVM_PROFILE_FILE=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profraw $<TARGET_FILE:unit_tests>
    COMMAND llvm-profdata merge -sparse ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profraw -o ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profdata
)

add_custom_target(cov-show
    COMMAND llvm-cov show $<TARGET_FILE:unit_tests> -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profdata -show-line-counts-or-regions
    DEPENDS generate-prof)

add_custom_target(cov-report
    COMMAND llvm-cov report $<TARGET_FILE:unit_tests> -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profdata "${CMAKE_SOURCE_DIR}/include"
    DEPENDS generate-prof)

add_custom_target(cov-html
    COMMAND llvm-cov show $<TARGET_FILE:unit_tests>
          -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profdata
          -show-line-counts-or-regions
          -output-dir=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests
          -format="html"
          "${CMAKE_SOURCE_DIR}/include"
    DEPENDS generate-prof)

add_custom_target(cov-lcov
    COMMAND llvm-cov export -format=lcov -instr-profile=${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.profdata $<TARGET_FILE:unit_tests> "${CMAKE_SOURCE_DIR}/include" > ${CMAKE_COVERAGE_OUTPUT_DIRECTORY}/unit_tests.lcov
    DEPENDS generate-prof)
