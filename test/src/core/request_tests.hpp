#include <catch.hpp>
#include <clement/clement.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/parser.hpp>

SCENARIO("clement::request can parse requests properly", "[core]") {
    GIVEN("A simple HTTP request header") {
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

        WHEN("The request header is parsed") {
            clement::request req;
            req.parse_header(request_header);
            THEN("Request target is parsed correctly") {
                REQUIRE(req.target() == "/api");
            }
            THEN("ContentType header is parsed correctly") {
                REQUIRE(req.content_type() == "application/json");
            }
            THEN("The verb of the request is returned correctly as an enum") {
                REQUIRE(req.verb() == boost::beast::http::verb::get);
            }
            THEN("The verb of the request is returned correctly as a std::string") {
                REQUIRE(req.verb_string() == "GET");
            }
        }

        WHEN("The request target does not have any path parameters") {
            clement::request req;
            req.parse_header(request_header);
            THEN("Retrieving path parameters returns an empty container") {
                REQUIRE(req.path_params().size() == 0);
            }
        }

        WHEN("A non-existing header is retrieved from the request") {
            clement::request req;
            req.parse_header(request_header);
            std::string result = req.header("Random-Header");
            THEN("An empty std::string instance is returned") {
                REQUIRE(result.empty());
            }
        }

        WHEN("All headers are retrieved from the request") {
            clement::request req;
            req.parse_header(request_header);
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
            clement::request req;
            req.parse_header(request_header);
            auto qparams = req.query_params();
            THEN("The returned query parameters are correct") {
                REQUIRE(qparams.size() == 1);
                REQUIRE(qparams.at("q1") == "value1");
            }
        }
    }
}