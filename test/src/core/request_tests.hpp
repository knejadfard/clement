#include <catch.hpp>
#include <clement/clement.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/parser.hpp>

SCENARIO("clement::request can parse requests properly", "[core]") {
    GIVEN("A simple HTTP request header") {
        boost::asio::io_context ioc;
        boost::beast::test::stream test_stream{ioc, "GET / HTTP/1.1\r\n"
                                                    "Host: localhost\r\n"
                                                    "User-Agent: test\r\n"
                                                    "Content-Length: 0\r\n"
                                                    "Content-Type: application/json\r\n"
                                                    "\r\n"};
        boost::beast::http::request_parser<boost::beast::http::empty_body> header_parser;
        boost::system::error_code ec;
        header_parser.put(test_stream.buffer().data(), ec);
        boost::beast::http::request<boost::beast::http::empty_body> request_header = header_parser.get();

        WHEN("clement::request parses the request header") {
            clement::request req;
            req.parse_header(request_header);
            THEN("Request target is parsed correctly") {
                REQUIRE(req.target() == "/");
            }
            THEN("ContentType header is parsed correctly") {
                REQUIRE(req.content_type() == "application/json");
            }
        }

        WHEN("clement::request is asked for a header that it does not have") {
            clement::request req;
            req.parse_header(request_header);
            std::string result = req.header("Random-Header");
            THEN("An empty std::string instance is returned") {
                REQUIRE(result.empty());
            }
        }

        WHEN("Retrieving all headers from a clement::request object") {
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
    }
}