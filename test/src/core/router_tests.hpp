#include "clement/clement.hpp"
#include "catch.hpp"

#include <functional>
#include <string>

// The value of this global variable is modified by test handlers and is checked
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

SCENARIO("clement::router is able to route requests correctly", "[core]") {
    GIVEN("A request object and router with two endpoints configured at /api/test1 and /api/test2") {
        clement::request parser{};
        int i = 0;
        clement::basic_writer<int> writer{i};

        clement::basic_router<int> r;
        r.get("/api/test1", &handler1);
        r.get("/api/test2", &handler2);

        WHEN("The router is requested to find a route for /api/test1") {
            std::function<void(clement::request&, clement::basic_writer<int>&)> test_handler =
                r.get_handler(clement::route{"GET", "/api/test1"});
            test_handler(parser, writer);
            THEN("The correct handler function gets called") {
                REQUIRE(val == 1);
            }
        }
    }
}