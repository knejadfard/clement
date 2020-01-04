#include <boost/test/included/unit_test.hpp>

#include "server/path.hpp"
#include "server/route.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(route_test_suite)

BOOST_AUTO_TEST_CASE(route_get_path) {
    server::route r = server::route("GET", "/test/path");
    server::path p = r.get_path();
    BOOST_TEST(p.get_string() == "/test/path");
}

BOOST_AUTO_TEST_CASE(route_verb_string) {
    server::route r = server::route("GET", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "GET");
}

BOOST_AUTO_TEST_CASE(route_post_method) {
    server::route r = server::route("POST", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "POST");
}

BOOST_AUTO_TEST_CASE(route_put_method) {
    server::route r = server::route("PUT", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "PUT");
}

BOOST_AUTO_TEST_CASE(route_del_method) {
    server::route r = server::route("DELETE", "/test/path");
    std::string method = r.verb_string();
    BOOST_TEST(method == "DELETE");
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_1) {
    server::route r1 = server::route("GET", "/test/path/method");
    server::route r2 = server::route("GET", "/test/path/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_CASE(route_comp_no_param_2) {
    server::route r1 = server::route("GET", "/test/path/method1");
    server::route r2 = server::route("GET", "/test/path/method2");
    BOOST_TEST(r1 != r2);
}

BOOST_AUTO_TEST_CASE(route_comp_with_param) {
    server::route r1 = server::route("GET", "/test/path/:param1/method");
    server::route r2 = server::route("GET", "/test/path/:param2/method");
    BOOST_TEST(r1 == r2);
}

BOOST_AUTO_TEST_SUITE_END()
