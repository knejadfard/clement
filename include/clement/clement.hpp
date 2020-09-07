#ifndef CLEMENT_CLEMENT_HPP
#define CLEMENT_CLEMENT_HPP

#include "constants.hpp"
#include "http_server.hpp"
#include "logger.hpp"
#include "path.hpp"
#include "request.hpp"
#include "response_builder.hpp"
#include "route.hpp"
#include "router.hpp"
#include "utility.hpp"
#include "writer.hpp"
#include <boost/beast/http.hpp>

namespace clement {

    namespace http = boost::beast::http;

}

#endif