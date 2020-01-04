#include "server/server.hpp"
#include "handler.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

    try {
        server::router r;
        r.get("/api/hello", handler{});
        server::server s("127.0.0.1", 8001);
        s.set_router(r);
        s.listen();
    } catch (std::exception const& e) {
        server::main_logger.error("Fatal error: " + std::string{e.what()});
        return EXIT_FAILURE;
    }

    return 0;
}
