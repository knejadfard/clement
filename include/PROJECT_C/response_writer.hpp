#pragma once

#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>
#include "logger.hpp"

namespace PROJECT_C {

namespace http = boost::beast::http;

template <class Stream>
class basic_response_writer
{

    Stream& stream_;

public:

    explicit basic_response_writer(Stream& stream)
        : stream_{stream}
    {
    }

    template <typename Body>
    void
    write(http::response<Body> response) const
    {
        http::response_serializer<Body> sr{response};
        http::write(stream_, sr);
    }
};

using response_writer = basic_response_writer<boost::asio::ip::tcp::socket>;

} // namespace PROJECT_C
