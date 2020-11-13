#include "clement/clement.hpp"
#include "catch.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>

SCENARIO("clement::writer can successfully write to a stream", "[core]") {
    GIVEN("Two test streams connected to each other and a clement::basic_writer bound to the input stream") {
        boost::asio::io_context ioc;
        boost::beast::test::stream input_stream{ioc};
        boost::beast::test::stream output_stream{ioc};
        input_stream.connect(output_stream);
        clement::basic_writer<boost::beast::test::stream> writer{input_stream};
        WHEN("The writer writes a response object to the bound stream") {
            clement::response_builder response_builder;
            response_builder.body("This is a test.");
            response_builder.result(boost::beast::http::status::ok);
            auto response = response_builder.build();
            writer.write(response);
            THEN("The output that gets written to the stream is correct") {
                REQUIRE(output_stream.str() ==
                        "HTTP/1.1 200 OK\r\n"
                        "access-control-allow-origin: *\r\n"
                        "Content-Length: 15\r\n"
                        "\r\n"
                        "This is a test.");
            }
        }
    }
}