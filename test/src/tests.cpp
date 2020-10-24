/*
 * This is the main file to setup Catch2 for all unit tests. This is being done to be able to have
 * the definition of CATCH_CONFIG_MAIN exactly in one source file, as required by Catch2.
 */

#define CATCH_CONFIG_MAIN

#include "core/path_tests.hpp"
#include "core/request_tests.hpp"
#include "utility_tests.hpp"