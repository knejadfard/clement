#pragma once

#include "server/server.hpp"
#include <fstream>
#include <map>
#include <sstream>

class handler {

  public:
    handler() {}

    void operator()(server::request& request, server::socket_writer& writer) {
        server::main_logger.info("Handling request: " + std::string(request));
        std::map<std::string, std::string> params = request.query_params();
        for (std::map<std::string, std::string>::const_iterator it = params.cbegin();
             it != params.cend(); ++it) {
            server::main_logger.debug("Param: " + it->first + "=" + it->second);
        }

        std::string content_type = request.content_type();
        if (content_type == server::constants::octet_stream) {
            std::ofstream out("output.txt");
            out << request.stream().rdbuf();
            out.close();
        }

        server::response_builder res;
        res.result(server::http::status::ok);
        res.body("Hello World!");
        writer.write(res.build());
    }
};
