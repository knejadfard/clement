#ifndef CLEMENT_CLEMENT_HPP
#define CLEMENT_CLEMENT_HPP

#include "clement/core/constants.hpp"
#include "clement/core/http_server.hpp"
#include "clement/core/logger.hpp"
#include "clement/core/path.hpp"
#include "clement/core/request.hpp"
#include "clement/core/response_builder.hpp"
#include "clement/core/route.hpp"
#include "clement/core/router.hpp"
#include "clement/core/utility.hpp"
#include "clement/core/writer.hpp"
#include <boost/beast/http.hpp>

namespace clement {

    namespace http = boost::beast::http;

}

#endif