Clement is a C++ library built on top of Boost.Beast to provide high-level facilities for writing client/clement applications.

# Build Status
[![Build Status](https://travis-ci.com/knejadfard/clement.svg?token=BQuV76EB4xzpZzDPez3Z&branch=master)](https://travis-ci.com/knejadfard/clement) [![codecov](https://codecov.io/gh/knejadfard/clement/branch/master/graph/badge.svg)](https://codecov.io/gh/knejadfard/clement)

The build matrix:
- Clang 10 and Boost 1.72.0 on Linux
- GCC 9 and Boost 1.72.0 on Linux

# Motivation
It was a few years back when the library Boost.Beast was released, which builds on top of Boost.Asio to provide low-level facilities for writing networking libraries.
With the C++ Networking TS in the works (still), and considering the fact that it is based on Boost.Asio, it felt completely natural to have a higher-level C++ library for writing clement and client applications. However, my search at the time did not show anything specific results. While there are some good C++ libraries out there for writing client/clement applications, none of them are built using Boost.Beast. This all led me to think about "What if I write one?" and here we are.


# Dependencies
## LLVM/Clang
The following dependencies are linked statically:
- libc++abi
- libc++
- libunwind
  - liblzma (needed by libunwind)

The following dependencies are linked dynamically:
- libm
- libc
- libdl

## GCC/G++
The following dependency is linked statically:
- libgcc

The following dependencies are linked dynamically:
- libm
- libc
- libgcc\_s

# Build
```
mkdir build
cd build
cmake ../
```

# Examples
There is currently 1 example program available in the `examples/` folder.
