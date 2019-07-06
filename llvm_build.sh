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

# The following environment variables must be present
#     LLVM_ROOT -> pointing to the root directory of LLVM toolchain that has clang and clang++
#     BOOST_ROOT -> pointing to the root directory of the compiled Boost library

# --------------------------------------------------------------------------------------------------
# Run CMake
# --------------------------------------------------------------------------------------------------
CMAKE_ENV="CC=${LLVM_ROOT}/bin/clang CXX=${LLVM_ROOT}/bin/clang++ "
env ${CMAKE_ENV} cmake -DBOOST_ROOT=${BOOST_ROOT} ../

# --------------------------------------------------------------------------------------------------
# Build
# --------------------------------------------------------------------------------------------------
make -j ${NUM_JOBS}

popd
