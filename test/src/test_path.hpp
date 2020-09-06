#include <boost/test/included/unit_test.hpp>

#include "clement/path.hpp"
#include <string>
#include <vector>

BOOST_AUTO_TEST_SUITE(path_test_suite)

BOOST_AUTO_TEST_CASE(path_empty_constructor) {
    clement::path p;
    p.set("test/path");
    std::string returned_path = p.get_string();
    BOOST_TEST(returned_path == "/test/path");
}

BOOST_AUTO_TEST_CASE(path_concatenation_size) {
    clement::path p{"p1/p2/p3/p4"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp.size() == 4);
}

BOOST_AUTO_TEST_CASE(path_concatenation_elements) {
    clement::path p{"p1/p2/p3/p4"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp == std::vector<std::string>({"p1", "p2", "p3", "p4"}));
}

BOOST_AUTO_TEST_CASE(path_ignore_trailing_slash) {
    clement::path p{"p1/p2/"};
    std::vector<std::string> vp = p.get_fragments();
    BOOST_TEST(vp == std::vector<std::string>({"p1", "p2"}));
}

BOOST_AUTO_TEST_CASE(path_equality_1) {
    clement::path p1{"/api/test"};
    clement::path p2{"/api/test"};
    BOOST_TEST(p1 == p2);
}

BOOST_AUTO_TEST_CASE(path_equality_2) {
    clement::path p1{"/api/test"};
    clement::path p2{"api/test"};
    BOOST_TEST(p1 == p2);
}

BOOST_AUTO_TEST_CASE(path_inequality) {
    clement::path p1{"/api/test1"};
    clement::path p2{"/api/test2"};
    BOOST_TEST(p1 != p2);
}

BOOST_AUTO_TEST_SUITE_END()
