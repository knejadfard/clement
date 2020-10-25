#include <catch.hpp>
#include <clement/core/path.hpp>

SCENARIO("clement::path works as expected", "[core]") {
    std::string path_str1{"p1/p2/p3/p4/"};
    GIVEN("The path string " + path_str1) {
        WHEN("path::set is called") {
            clement::path p;
            p.set(path_str1);

            std::string expected = "/p1/p2/p3/p4";
            THEN("Calling get_string on the path object returns " + expected) {
                REQUIRE(p.get_string() == expected);
            }
        }
    }

    GIVEN("A path object initialized with string " + path_str1) {
        clement::path p{path_str1};
        WHEN("path::get_fragments is called") {
            std::vector<std::string> fragments = p.get_fragments();
            THEN("The fragments vector has 4 elements") { REQUIRE(fragments.size() == 4); }
            THEN("The fragments vector has the expected elements") {
                REQUIRE(fragments == std::vector<std::string>({"p1", "p2", "p3", "p4"}));
            }
            THEN("The last fragment does not include the trailing slash") {
                REQUIRE(fragments.at(3) == "p4");
            }
        }
    }

    GIVEN("Two equivalent instances of path") {
        clement::path p1{"/api/test"};
        clement::path p2{"/api/test"};
        WHEN("The two path instances are compared") {
            bool result = p1 == p2;
            THEN("The result is true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Two equivalent instances of path with one of them missing leading /") {
        clement::path p1{"/api/test"};
        clement::path p2{"api/test"};
        WHEN("The two path instances are compared") {
            bool result = p1 == p2;
            THEN("The result is true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Two non-equal instances of path") {
        clement::path p1{"/api/test1"};
        clement::path p2{"/api/test2"};
        WHEN("The two path instances are compared") {
            bool result = p1 == p2;
            THEN("The result is false") { REQUIRE(result == false); }
        }
    }
}