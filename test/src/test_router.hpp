#include <boost/test/included/unit_test.hpp>

#include <functional>
#include <string>
#include <vector>

#include "clement/request.hpp"
#include "clement/route.hpp"
#include "clement/router.hpp"
#include "clement/writer.hpp"

BOOST_AUTO_TEST_SUITE(router_test_suite)

// the value of this global variable is modified by test handlers and is checked
// to ensure that the correct handler has been called.
int val = 0;

void handler1(clement::request& req, clement::basic_writer<int>& res) {
    val = 1;
    return;
}

void handler2(clement::request& req, clement::basic_writer<int>& res) {
    val = 2;
    return;
}

BOOST_AUTO_TEST_CASE(router_test_int) {
    clement::basic_router<int> r;
    r.get("/api/test1", &handler1);
    r.get("/api/test2", &handler2);

    std::function<void(clement::request&, clement::basic_writer<int>&)> test_handler =
        r.get_handler(clement::route{"GET", "/api/test1"});

    clement::request parser{};

    int i = 0;
    clement::basic_writer<int> writer{i};

    test_handler(parser, writer);

    BOOST_TEST(val == 1);
}

BOOST_AUTO_TEST_SUITE_END()
