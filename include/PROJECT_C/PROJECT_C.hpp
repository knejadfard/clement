#pragma once

#include <boost/beast/http.hpp>
#include "logger.hpp"
#include "constants.hpp"
#include "request_parser.hpp"
#include "response_writer.hpp"
#include "response_builder.hpp"
#include "path.hpp"
#include "route.hpp"
#include "router.hpp"
#include "utility.hpp"
#include "server.hpp"

namespace PROJECT_C {

    namespace http = boost::beast::http;

}
