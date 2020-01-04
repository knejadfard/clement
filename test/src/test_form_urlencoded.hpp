#include <boost/test/included/unit_test.hpp>

#include "server/utility/form_urlencoded.hpp"
#include <map>
#include <string>

BOOST_AUTO_TEST_SUITE(form_urlencoded_test_suite)

BOOST_AUTO_TEST_CASE(test_parsing) {
    std::string str{"name1=kian&name2=nejadfard&age=25"};
    server::utility::form_urlencoded form{str};
    BOOST_TEST(form.params()["name1"] == "kian");
}

BOOST_AUTO_TEST_CASE(test_parsing_with_decode) {
    std::string str{"fullname=kian%20nejadfard&age=25"};
    server::utility::form_urlencoded form{str};
    BOOST_TEST(form.params()["fullname"] == "kian nejadfard");
}

BOOST_AUTO_TEST_SUITE_END()
