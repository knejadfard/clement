#include <boost/test/included/unit_test.hpp>

#include "clement/utility/generate_string.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(generate_string_test_suite)

BOOST_AUTO_TEST_CASE(test_gen) {
    std::string str = clement::utility::generate_string(8);
    BOOST_TEST(str.size() == 8);
}

BOOST_AUTO_TEST_SUITE_END()
