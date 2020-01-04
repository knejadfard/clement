#include <boost/test/included/unit_test.hpp>

#include "server/utility/base64.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(base64_test_suite)

BOOST_AUTO_TEST_CASE(test_encoding) {
    std::string raw{"username=kian&password=password"};
    std::string encoded = server::utility::encode64(raw);
    BOOST_TEST(encoded == "dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA==");
}

BOOST_AUTO_TEST_CASE(test_decoding) {
    std::string encoded{"dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA=="};
    std::string decoded = server::utility::decode64(encoded);
    BOOST_TEST(decoded == "username=kian&password=password");
}

BOOST_AUTO_TEST_SUITE_END()
