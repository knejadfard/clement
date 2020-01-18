#ifndef SERVER_TEST_REQUEST_HPP
#define SERVER_TEST_REQUEST_HPP

#include <boost/test/included/unit_test.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>
#include "server/request.hpp"

BOOST_AUTO_TEST_SUITE(request_parser_test_suite)

BOOST_AUTO_TEST_CASE(simple_header_test) {
    boost::asio::io_context ioc;
    boost::beast::test::stream test_stream{ioc, "GET / HTTP/1.1\r\n"
                                                "Host: localhost\r\n"
                                                "User-Agent: test\r\n"
                                                "Content-Length: 0\r\n"
                                                "Content-Type: application/json\r\n"
                                                "\r\n"};
    server::request req;
    boost::beast::http::request_parser<boost::beast::http::empty_body> header_parser;
    boost::system::error_code ec;
    header_parser.put(test_stream.buffer().data(), ec);
    boost::beast::http::request<boost::beast::http::empty_body> request_header = header_parser.get();
    req.parse_header(request_header);
    BOOST_TEST(req.target() == "/");
    BOOST_TEST(req.content_type() == "application/json");
}

BOOST_AUTO_TEST_SUITE_END()

#endif //SERVER_TEST_REQUEST_HPP