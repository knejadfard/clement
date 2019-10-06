#include "PROJECT_C/PROJECT_C.hpp"
#include "handler.hpp"
#include <iostream>

int main(int argc, char* argv[]) {

  try {
    PROJECT_C::router r;
    r.get("/api/hello", handler{});
    PROJECT_C::server s("127.0.0.1", 8001);
    s.set_router(r);
    s.listen();
  } catch (std::exception const& e) {
    PROJECT_C::main_logger.error("Fatal error: " + std::string{e.what()});
    return EXIT_FAILURE;
  }

  return 0;
}
