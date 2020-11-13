/*
 * This is the main file to setup Catch2 for all unit tests. This is being done to be able to have
 * the definition of CATCH_CONFIG_MAIN exactly in one source file, as required by Catch2.
 */

#define CATCH_CONFIG_MAIN

#include "core/path_tests.hpp"
#include "core/request_tests.hpp"
#include "core/logger_tests.hpp"
#include "core/route_tests.hpp"
#include "core/router_tests.hpp"
#include "core/writer_tests.hpp"
#include "core/http_server_tests.hpp"

#include "utility_tests.hpp"