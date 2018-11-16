#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <string>
#include "PROJECT_C/route.hpp"
#include "PROJECT_C/path.hpp"

BOOST_AUTO_TEST_SUITE(route_test_suite)

BOOST_AUTO_TEST_CASE(route_get_path) {
    PROJECT_C::route r = PROJECT_C::route("GET", "/test/path");
    PROJECT_C::path p = r.get_path();
    BOOST_TEST(p.get_string() == "/test/path");
}

BOOST_AUTO_TEST_CASE(route_verb_string) {
    PROJECT_C::route r = PROJECT_C::route("GET", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "GET");
}

BOOST_AUTO_TEST_CASE(route_post_method) {
    PROJECT_C::route r = PROJECT_C::route("POST", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "POST");
}

BOOST_AUTO_TEST_CASE(route_put_method) {
    PROJECT_C::route r = PROJECT_C::route("PUT", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "PUT");
}

BOOST_AUTO_TEST_CASE(route_del_method) {
    PROJECT_C::route r = PROJECT_C::route("DELETE", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "DELETE");
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_1) {
    PROJECT_C::route r1 = PROJECT_C::route("GET", "/test/path/method");
    PROJECT_C::route r2 = PROJECT_C::route("GET", "/test/path/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_2) {
    PROJECT_C::route r1 = PROJECT_C::route("GET", "/test/path/method1");
    PROJECT_C::route r2 = PROJECT_C::route("GET", "/test/path/method2");
    BOOST_TEST(r1 != r2);
}

BOOST_AUTO_TEST_CASE(route_comp_with_param) {
    PROJECT_C::route r1 = PROJECT_C::route("GET", "/test/path/:param1/method");
    PROJECT_C::route r2 = PROJECT_C::route("GET", "/test/path/:param2/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_SUITE_END()
