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

BOOST_AUTO_TEST_SUITE_END()
