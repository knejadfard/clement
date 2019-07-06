#pragma once

#include <map>
#include <fstream>
#include <sstream>
#include "PROJECT_C/PROJECT_C.hpp"

class handler {

public:

    handler()
    {
    }

    void operator()(PROJECT_C::request_parser& request, PROJECT_C::socket_writer& writer)
    {
        PROJECT_C::main_logger.info("Handling request: " + std::string(request));
        std::map<std::string, std::string> params = request.query_params();
        for(std::map<std::string, std::string>::const_iterator it = params.cbegin(); it != params.cend(); ++it) {
            PROJECT_C::main_logger.debug("Param: " + it->first + "=" + it->second);
        }

        std::string content_type = request.content_type();
        if(content_type == PROJECT_C::constants::octet_stream) {
            std::ofstream out("output.txt");
            out << request.stream().rdbuf();
            out.close();
        }

        PROJECT_C::response_builder res;
        res.result(PROJECT_C::http::status::ok);
        res.body("Hello World!");
        writer.write(res.build());
    }
};
