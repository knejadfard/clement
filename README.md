This repository is a C++ library that builds on top of Boost::Beast and allows users to develop HTTP (and in future, Websocket) services.

# Build Status
[![Build Status](https://travis-ci.com/knejadfard/server.svg?token=BQuV76EB4xzpZzDPez3Z&branch=master)](https://travis-ci.com/knejadfard/server) [![codecov](https://codecov.io/gh/knejadfard/server/branch/master/graph/badge.svg)](https://codecov.io/gh/knejadfard/server)

The build matrix:
- Clang 7 and Boost 1.72.0 on Linux
- GCC 9 and Boost 1.72.0 on Linux

# Dependencies
## LLVM/Clang
The following dependencies are linked statically:
- libc++abi
- libc++
- libunwind
  - liblzma (needed by libunwind)

The following libraries are linked dynamically:
- libm
- libc
- libdl

## GCC/G++
The following library is linked statically:
- libgcc

The following libraries are linked dynamically:
- libm
- libc
- libgcc\_s

# Motivation
TODO

# Build

# Usage
TODO
