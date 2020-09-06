#include <boost/test/included/unit_test.hpp>

#include "clement/path.hpp"
#include "clement/route.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(route_test_suite)

BOOST_AUTO_TEST_CASE(route_get_path) {
    clement::route r = clement::route("GET", "/test/path");
    clement::path p = r.get_path();
    BOOST_TEST(p.get_string() == "/test/path");
}

BOOST_AUTO_TEST_CASE(route_verb_string) {
    clement::route r = clement::route("GET", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "GET");
}

BOOST_AUTO_TEST_CASE(route_post_method) {
    clement::route r = clement::route("POST", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "POST");
}

BOOST_AUTO_TEST_CASE(route_put_method) {
    clement::route r = clement::route("PUT", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "PUT");
}

BOOST_AUTO_TEST_CASE(route_del_method) {
    clement::route r = clement::route("DELETE", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "DELETE");
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_1) {
    clement::route r1 = clement::route("GET", "/test/path/method");
    clement::route r2 = clement::route("GET", "/test/path/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_2) {
    clement::route r1 = clement::route("GET", "/test/path/method1");
    clement::route r2 = clement::route("GET", "/test/path/method2");
    BOOST_TEST(r1 != r2);
}

BOOST_AUTO_TEST_CASE(route_comp_with_param) {
    clement::route r1 = clement::route("GET", "/test/path/:param1/method");
    clement::route r2 = clement::route("GET", "/test/path/:param2/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_SUITE_END()
