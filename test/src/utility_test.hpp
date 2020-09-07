#include "catch.hpp"

#include "clement/utility/base64.hpp"
#include <clement/utility/form_urlencoded.hpp>
#include <clement/utility/generate_string.hpp>
#include <string>

SCENARIO("Strings can be encoded to base64", "[utility]") {
    GIVEN("A basic auth string with username and password values") {
        std::string raw{"username=kian&password=password"};
        WHEN("The string is encoded to base64") {
            std::string encoded = clement::utility::encode64(raw);
            THEN("The encoded value matches the expected value") {
                REQUIRE(encoded == "dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA==");
            }
        }
    }
}

SCENARIO("Strings can be decoded from base64", "[utility]") {
    GIVEN("A base64-encoded string") {
        std::string encoded{"dXNlcm5hbWU9a2lhbiZwYXNzd29yZD1wYXNzd29yZA=="};
        WHEN("The string is decoded from base64") {
            std::string decoded = clement::utility::decode64(encoded);
            THEN("The decoded value matches the expected value") {
                REQUIRE(decoded == "username=kian&password=password");
            }
        }
    }
}

SCENARIO("form_urlencoded can be used to consume form data", "[utility]") {
    GIVEN("A string with form data that is not url-encoded") {
        std::string nonencoded_string{"name=kian nejadfard&age=25"};
        WHEN("The string is used to construct a form_urlencoded object instance") {
            clement::utility::form_urlencoded encoded{nonencoded_string};
            THEN("Key/value pairs of the form data can be retrieved successfully") {
                REQUIRE(encoded.params()["name"] == "kian nejadfard");
                REQUIRE(encoded.params()["age"] == "25");
            }
        }
    }

    GIVEN("A string with form data that is url-encoded") {
        std::string encoded_string{"name=kian%20nejadfard&age=25"};
        WHEN("The string is used to construct a form_urlencoded object instance") {
            clement::utility::form_urlencoded encoded{encoded_string};
            THEN("Key/value pairs of the form data can be retrieved successfully") {
                REQUIRE(encoded.params()["name"] == "kian nejadfard");
                REQUIRE(encoded.params()["age"] == "25");
            }
        }
    }
}

SCENARIO("is_unreserved function can detect unreserved characters", "[utility]") {
    char a = 'a', two = '2', hyphen = '-', underscore = '_', dot = '.', tilda = '~', dollar = '$';
    GIVEN("Character 'a'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(a);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '2'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(two);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '-'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(hyphen);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '_'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(underscore);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '.'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(dot);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '~'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(tilda);
            THEN("It returns true") { REQUIRE(result == true); }
        }
    }

    GIVEN("Character '$'") {
        WHEN("is_unreserved is called") {
            bool result = clement::utility::is_unreserved(dollar);
            THEN("It returns true") { REQUIRE(result == false); }
        }
    }
}

SCENARIO("percent_encode function works as expected", "[utility]") {
    std::string input{"test string-_.~$"};
    std::string expected{"test%20string-_.~%24"};
    GIVEN("The string " + input) {
        WHEN("The string is passed to percent_encode function") {
            std::string actual = clement::utility::percent_encode(input);
            THEN("The result equals " + expected) { REQUIRE(actual == expected); }
        }
    }
}

SCENARIO("generate_string function can produce random strings", "[utility]") {
    WHEN("generate_string function is called to generate a string of specific length") {
        std::string str = clement::utility::generate_string(8);
        THEN("The produced string has the desired length") {
            REQUIRE(str.size() == 8);
        }
    }
}