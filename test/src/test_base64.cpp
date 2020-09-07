#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "clement/utility/base64.hpp"
#include <string>

SCENARIO("Strings can be encoded to base64", "[utility]") {
    GIVEN("A basic auth string with username and password values") {
        std::string raw{"username=kian&password=password"};
        WHEN("The string is encoded to base64") {
            std::string encoded = clement::utility::encode64(raw);
            THEN("The encoded value matches the expected value") {
                REQUIRE(encoded == "dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA==");
            }
        }
    }
}

SCENARIO("Strings can be decoded from base64", "[utility]") {
    GIVEN("A base64-encoded string") {
        std::string encoded{"dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA=="};
        WHEN("The string is decoded from base64") {
            std::string decoded = clement::utility::decode64(encoded);
            THEN("The decoded value matches the expected value") {
                REQUIRE(decoded == "username=kian&password=password");
            }
        }
    }
}
