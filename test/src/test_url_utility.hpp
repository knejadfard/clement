#include <boost/test/included/unit_test.hpp>

#include "clement/utility/url_utility.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(url_utility_test_suite)

BOOST_AUTO_TEST_CASE(is_unreserved) {
    bool result;

    result = clement::utility::is_unreserved('a');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('2');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('-');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('_');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('.');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('~');
    BOOST_TEST(result == true);

    result = clement::utility::is_unreserved('$');
    BOOST_TEST(result == false);
}

BOOST_AUTO_TEST_CASE(percent_encode) {
    std::string input{"test string-_.~$"};
    std::string expected{"test%20string-_.~%24"};
    std::string actual = clement::utility::percent_encode(input);
    BOOST_TEST(actual == expected);
}

BOOST_AUTO_TEST_SUITE_END()
