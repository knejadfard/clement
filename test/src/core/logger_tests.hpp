#include <clement/core/logger.hpp>
#include <catch.hpp>
#include <fstream>
#include <string>

SCENARIO("clement::logger is able to write to log files", "[core]") {
    GIVEN("An instance of clement::logger has been initialized") {
        // scoping ensures that it's ok to open the log file for verifying the tests
        clement::logger logger{"test.log"};

        WHEN("A string is logged at info level") {
            logger.info("test info log");
            THEN("The string is logged at info level") {
                std::fstream input{"test.log"};
                std::string line;
                std::getline(input, line);
                REQUIRE(line.find("[info] test info log") != std::string::npos);
            }
        }
    }

    GIVEN("An instance of clement::logger has been initialized") {
        // scoping ensures that it's ok to open the log file for verifying the tests
        clement::logger logger{"test.log"};

        WHEN("A string is logged at debug level") {
            logger.debug("test debug log");
            THEN("The string is logged at debug level") {
                std::fstream input{"test.log"};
                std::string line;
                std::getline(input, line);
                REQUIRE(line.find("[debug] test debug log") != std::string::npos);
            }
        }
    }

    GIVEN("An instance of clement::logger has been initialized") {
        // scoping ensures that it's ok to open the log file for verifying the tests
        clement::logger logger{"test.log"};

        WHEN("A string is logged at error level") {
            logger.error("test error log");
            THEN("The string is logged at error level") {
                std::fstream input{"test.log"};
                std::string line;
                std::getline(input, line);
                REQUIRE(line.find("[error] test error log") != std::string::npos);
            }
        }
    }
}