#include <boost/test/included/unit_test.hpp>

#include "server/logger.hpp"
#include <fstream>
#include <string>

BOOST_AUTO_TEST_SUITE(logger_test_suite)

BOOST_AUTO_TEST_CASE(log_info) {
    // scoping ensures that it's ok to open the log file for verifying the tests
    {
        server::logger logger{"test1.log"};
        logger.info("test");
    }
    std::fstream input{"test1.log"};
    std::string line;
    std::getline(input, line);
    BOOST_TEST(line.find("[info] test") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(log_error) {
    {
        server::logger logger{"test2.log"};
        logger.error("test");
    }
    std::fstream input{"test2.log"};
    std::string line;
    std::getline(input, line);
    BOOST_TEST(line.find("[error] test") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(log_debug) {
    {
        server::logger logger{"test3.log"};
        logger.debug("test");
    }
    std::fstream input{"test3.log"};
    std::string line;
    std::getline(input, line);
    BOOST_TEST(line.find("[debug] test") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
