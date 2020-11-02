#include <catch.hpp>
#include <clement/clement.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/parser.hpp>

SCENARIO("clement::request can parse requests properly", "[core]") {
    GIVEN("An HTTP request with target GET /api?q1=value1") {
        boost::asio::io_context ioc;
        boost::beast::test::stream test_stream{ioc, "GET /api?q1=value1 HTTP/1.1\r\n"
                                                    "Host: localhost\r\n"
                                                    "User-Agent: test\r\n"
                                                    "Content-Length: 0\r\n"
                                                    "Content-Type: application/json\r\n"
                                                    "\r\n"
                                                    "This is the body of the request"};
        boost::beast::http::request_parser<boost::beast::http::empty_body> header_parser;
        boost::system::error_code ec;
        header_parser.put(test_stream.buffer().data(), ec);
        boost::beast::http::request<boost::beast::http::empty_body> request_header = header_parser.get();

        clement::request req;
        req.parse_header(request_header);

        WHEN("The request target is retrieved") {
            std::string test_value = req.target();
            THEN("It has the correct value") { REQUIRE(test_value == "/api"); }
        }

        WHEN("The Content-Type header value is retrieved") {
            std::string test_value = req.content_type();
            THEN("ContentType header is parsed correctly") {
                REQUIRE(test_value == "application/json");
            }
        }

        WHEN("The verb of the request is retrieved as an enum value") {
            auto test_value = req.verb();
            THEN("The verb of the request is returned correctly as an enum") {
                REQUIRE(test_value == boost::beast::http::verb::get);
            }
        }

        WHEN("The verb of the request is retrieved as a std::string value") {
            std::string test_value = req.verb_string();
            THEN("The verb of the request is returned correctly as a std::string") {
                REQUIRE(test_value == "GET");
            }
        }

        WHEN("The path parameters are retrieved from the request") {
            int test_value = req.path_params().size();
            THEN("The returned container is empty") {
                REQUIRE(test_value == 0);
            }
        }

        WHEN("A non-existing header is retrieved from the request") {
            std::string result = req.header("Random-Header");
            THEN("An empty std::string instance is returned") {
                REQUIRE(result.empty());
            }
        }

        WHEN("All headers are retrieved from the request") {
            auto headers = req.headers();
            THEN("The returned headers are correct") {
                REQUIRE(headers.size() == 4);
                REQUIRE(headers.at(boost::beast::http::field::host) == "localhost");
                REQUIRE(headers.at(boost::beast::http::field::user_agent) == "test");
                REQUIRE(headers.at(boost::beast::http::field::content_length) == "0");
                REQUIRE(headers.at(boost::beast::http::field::content_type) == "application/json");
            }
        }

        WHEN("The query parameters are retrieved from the request") {
            auto qparams = req.query_params();
            THEN("The returned query parameters are correct") {
                REQUIRE(qparams.size() == 1);
                REQUIRE(qparams.at("q1") == "value1");
            }
        }
    }
}