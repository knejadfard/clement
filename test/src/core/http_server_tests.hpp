#include "clement/clement.hpp"
#include "catch.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/beast/_experimental/test/stream.hpp>

SCENARIO("TODO", "[core]") {
    GIVEN("TODO") {
        boost::asio::io_context ioc;
        boost::beast::test::stream input_stream{ioc};
        boost::beast::test::stream output_stream{ioc};
        input_stream.connect(output_stream);

        WHEN("TODO") {
            THEN("TODO") {
                REQUIRE(true);
            }
        }
    }
}