#include <catch.hpp>
#include <clement/core/path.hpp>
#include <sstream>

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
        WHEN("The two path instances are compared using operator==") {
            bool result = p1 == p2;
            THEN("The result is true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Two equivalent instances of path with one of them missing leading /") {
        clement::path p1{"/api/test"};
        clement::path p2{"api/test"};
        WHEN("The two path instances are compared using operator==") {
            bool result = p1 == p2;
            THEN("The result is true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Two non-equal instances of path") {
        clement::path p1{"/api/test1"};
        clement::path p2{"/api/test2"};
        WHEN("The two path instances are compared using operator==") {
            bool result = p1 == p2;
            THEN("The result is false") { REQUIRE(result == false); }
        }
    }

    GIVEN("Two instances of path with different number of fragments") {
        clement::path p1{"/api/v1/endpoint"};
        clement::path p2{"/api/endpoint"};
        WHEN("The two path instances are compared using operator==") {
            bool result = p1 == p2;
            THEN("The result is false") {
                REQUIRE(result == false);
            }
        }
    }

    GIVEN("Two instances of path that have different fragments") {
        clement::path p1{"/api/v1/endpoint"};
        clement::path p2{"/api/v2/endpoint"};
        WHEN("The two path instances are compared using operator!=") {
            bool result = p1 != p2;
            THEN("The result is true") {
                REQUIRE(result == true);
            }
        }
    }

    GIVEN("A path object that is default initialized without any path fragments") {
        clement::path p;
        WHEN("operator/ is used to set the path fragments using std::string instances") {
            std::string fragment1{"api"}, fragment2{"v1"};
            p = p / fragment1 / fragment2;
            THEN("The resulting path correctly represents the combination of path fragments") {
                REQUIRE(p.get_string() == "/api/v1");
            }
        }
    }

    GIVEN("A path object that is default initialized with path fragments") {
        clement::path p{"/api/v1"};
        WHEN("operator/ is used to set the path fragments using std::string instances") {
            std::string fragment{"endpoint"};
            p = p / fragment;
            THEN("The resulting path correctly represents the combination of path fragments") {
                REQUIRE(p.get_string() == "/api/v1/endpoint");
            }
        }
    }

    GIVEN("A path object that is default initialized without any path fragments") {
        clement::path p;
        WHEN("operator/ is used to set the path fragments using clement::path instances") {
            clement::path fragment1{"api"}, fragment2{"v1"};
            p = p / fragment1 / fragment2;
            THEN("The resulting path correctly represents the combination of path fragments") {
                REQUIRE(p.get_string() == "/api/v1");
            }
        }
    }

    GIVEN("A path object that is default initialized with path fragments") {
        clement::path p{"/api/v1"};
        WHEN("operator/ is used to set the path fragments using a clement::path that has multiple fragments") {
            clement::path fragment{"path/endpoint"};
            p = p / fragment;
            THEN("The resulting path correctly represents the combination of path fragments") {
                REQUIRE(p.get_string() == "/api/v1/path/endpoint");
            }
        }
    }

    GIVEN("A path that is default initialized with path fragments") {
        clement::path p{"/api/v1/endpoint"};
        WHEN("operator<< is used to dump the path fragments into a std::ostream") {
            std::stringstream stream;
            stream << p;
            THEN("The stream content matches the string version of the path") {
                REQUIRE(stream.str() == p.get_string());
            }
        }
    }

    GIVEN("A path object that is initialized with fragments") {
        clement::path p{"/api/v1/endpoint"};
        WHEN("It is converted to a std::string") {
            std::string result = (std::string) p;
            THEN("The resulting string is correct") {
                REQUIRE(result == "/api/v1/endpoint");
            }
        }
    }
}