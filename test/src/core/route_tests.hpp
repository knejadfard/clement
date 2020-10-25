#include "clement/clement.hpp"
#include "catch.hpp"
#include <string>
#include <vector>

SCENARIO("clement::route is functioning correctly", "[core]") {
    GIVEN("GET /test/path as the route") {
        clement::route r = clement::route("GET", "/test/path");
        WHEN("An instance of clement::path is returned by get_path()") {
            clement::path p = r.get_path();
            THEN("The returned path is equal to \"/test/path\"") {
                REQUIRE(p.get_string() == "/test/path");
            }
        }
        WHEN("The clement::http::verb is retrieved from the route") {
            clement::http::verb verb = r.verb();
            THEN("The HTTP verb is GET") {
                REQUIRE(verb == clement::http::verb::get);
            }
        }
    }

    GIVEN("POST /test/path as the route") {
        clement::route r = clement::route("POST", "/test/path");
        WHEN("The clement::http::verb is retrieved from the route") {
            clement::http::verb verb = r.verb();
            THEN("The HTTP verb is POST") {
                REQUIRE(verb == clement::http::verb::post);
            }
        }
    }

    GIVEN("PUT /test/path as the route") {
        clement::route r = clement::route("PUT", "/test/path");
        WHEN("The clement::http::verb is retrieved from the route") {
            clement::http::verb verb = r.verb();
            THEN("The HTTP verb is PUT") {
                REQUIRE(verb == clement::http::verb::put);
            }
        }
    }

    GIVEN("DELETE /test/path as the route") {
        clement::route r = clement::route("DELETE", "/test/path");
        WHEN("The clement::http::verb is retrieved from the route") {
            clement::http::verb verb = r.verb();
            THEN("The HTTP verb is DELETE") {
                REQUIRE(verb == clement::http::verb::delete_);
            }
        }
    }

    GIVEN("Two route instances with identical HTTP verb and paths") {
        clement::route r1 = clement::route("GET", "/test/path/method");
        clement::route r2 = clement::route("GET", "/test/path/method");
        WHEN("The two routes are compared to each other") {
            THEN("The result is true") {
                REQUIRE(r1 == r2);
            }
        }
    }

    GIVEN("Two route instances with identical paths but different HTTP verbs") {
        clement::route r1 = clement::route("GET", "/test/path/method");
        clement::route r2 = clement::route("PUT", "/test/path/method");
        WHEN("The two routes are compared to each other") {
            THEN("The result is false") {
                REQUIRE(r1 != r2);
            }
        }
    }

    GIVEN("Two route instances with identical HTTP verb and different paths") {
        clement::route r1 = clement::route("GET", "/test/path/method1");
        clement::route r2 = clement::route("GET", "/test/path/method2");
        WHEN("The two routes are compared to each other") {
            THEN("The result is false") {
                REQUIRE(r1 != r2);
            }
        }
    }

    GIVEN("Two route instances with identical HTTP verb and paths that only differ in query parameter name") {
        clement::route r1 = clement::route("GET", "/test/path/:param1/method");
        clement::route r2 = clement::route("GET", "/test/path/:param2/method");
        WHEN("The two routes are compared to each other") {
            THEN("The result is true") {
                REQUIRE(r1 == r2);
            }
        }
    }
}