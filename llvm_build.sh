#!/bin/bash

# --------------------------------------------------------------------------------------------------
# Initialize the environment
# --------------------------------------------------------------------------------------------------
rm -rf ./build
mkdir build
pushd build

NUM_JOBS=1
if [ $# -gt 0 ]; then
    if [ ! -z $1 -a $1 -gt 0 ]; then
        NUM_JOBS=$1
    fi
fi

echo "Number of make jobs to run: $NUM_JOBS"

# --------------------------------------------------------------------------------------------------
# Configure CMake
# --------------------------------------------------------------------------------------------------
# The following environment variables must be present
#     LLVM_ROOT -> pointing to the root directory of LLVM toolchain that has clang and clang++
#     BOOST_ROOT -> pointing to the root directory of the compiled Boost library

# Build a list of CMake options
CMAKE_OPTS="-DBUILD_DOC=OFF -DCODE_COVERAGE=ON"
CMAKE_OPTS="${CMAKE_OPTS} -DBOOST_ROOT=${BOOST_ROOT}"
CMAKE_OPTS="${CMAKE_OPTS} -DCMAKE_BUILD_TYPE=Debug"
CMAKE_OPTS="${CMAKE_OPTS} -DCMAKE_COVERAGE_OUTPUT_DIRECTORY=./coverage"
CMAKE_OPTS="${CMAKE_OPTS} -DLLVM_ROOT=${LLVM_ROOT}"

CMAKE_ENV="CC=${LLVM_ROOT}/bin/clang CXX=${LLVM_ROOT}/bin/clang++ "

# --------------------------------------------------------------------------------------------------
# Run CMake
# --------------------------------------------------------------------------------------------------
env ${CMAKE_ENV} cmake ${CMAKE_OPTS} ../

# --------------------------------------------------------------------------------------------------
# Build
# --------------------------------------------------------------------------------------------------
make -j ${NUM_JOBS}

popd
