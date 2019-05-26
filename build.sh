#!/bin/bash

rm -rf ./build
mkdir build
pushd build

CC=~/binaries/llvm-8.0.0/bin/clang CXX=~/binaries/llvm-8.0.0/bin/clang++ \
    cmake -DBOOST_ROOT=/home/kian/binaries/boost-1.68-clang ../

make -j 4

popd
