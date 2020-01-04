#include <boost/test/included/unit_test.hpp>

#include "server/path.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(path_test_suite)

BOOST_AUTO_TEST_CASE(path_empty_constructor) {
    server::path p;
    p.set("test/path");
    std::string returned_path = p.get_string();
    BOOST_TEST(returned_path == "/test/path");
}

BOOST_AUTO_TEST_CASE(path_concatenation_size) {
    server::path p{"p1/p2/p3/p4"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp.size() == 4);
}

BOOST_AUTO_TEST_CASE(path_concatenation_elements) {
    server::path p{"p1/p2/p3/p4"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp == std::vector<std::string>({"p1", "p2", "p3", "p4"}));
}

BOOST_AUTO_TEST_CASE(path_ignore_trailing_slash) {
    server::path p{"p1/p2/"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp == std::vector<std::string>({"p1", "p2"}));
}

BOOST_AUTO_TEST_CASE(path_equality_1) {
    server::path p1{"/api/test"};
    server::path p2{"/api/test"};
    BOOST_TEST(p1 == p2);
}

BOOST_AUTO_TEST_CASE(path_equality_2) {
    server::path p1{"/api/test"};
    server::path p2{"api/test"};
    BOOST_TEST(p1 == p2);
}

BOOST_AUTO_TEST_CASE(path_inequality) {
    server::path p1{"/api/test1"};
    server::path p2{"/api/test2"};
    BOOST_TEST(p1 != p2);
}

BOOST_AUTO_TEST_SUITE_END()
