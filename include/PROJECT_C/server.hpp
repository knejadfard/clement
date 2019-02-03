#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <functional>
#include <chrono>
#include <queue>
#include <boost/system/system_error.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/config.hpp>
#include "logger.hpp"
#include "response_writer.hpp"
#include "router.hpp"
#include "request_parser.hpp"

namespace PROJECT_C {

using tcp = boost::asio::ip::tcp;
using socket_type = tcp::socket;
namespace http = boost::beast::http;

class server {

    std::string address_;
    unsigned short port_;

    std::mutex mutex_;
    std::condition_variable handler_condition_;
    std::queue<socket_type> incoming_requests_;

    // Thread that picks up accepted sockets from the queue and processes them
    // one by one.
    std::thread handler_thread_;

    router router_;

public:

    server(std::string const& address, unsigned short const& port)
            : address_{address},
            port_{port},
            handler_thread_{&server::work, this},
            router_{} {
    }

    // TODO comments
    void listen();

    // TODO comments
    void work();

    // TODO comments
    void set_router(router& r);

};

void server::listen() {
    main_logger.info("Started listening on " +
            address_ + ":" + std::to_string(port_));

    boost::asio::io_context ioc{1};

    auto const address = boost::asio::ip::make_address(address_);

    tcp::acceptor acceptor{ioc, {address, port_}};
    for(;;) {
        tcp::socket socket{ioc};

        acceptor.accept(socket);

        std::unique_lock<std::mutex> lock(mutex_);
        incoming_requests_.push(std::move(socket));
        main_logger.debug("Added new socket to queue");
        lock.unlock();

        handler_condition_.notify_all();
    }
}

void server::work() {
    while(true) {
        std::unique_lock<std::mutex> lock(mutex_);
        // block thread until there are items to process
        handler_condition_.wait(lock,
                [this](){ return !incoming_requests_.empty(); });

        boost::asio::ip::tcp::socket socket = std::move(
                incoming_requests_.front());
        incoming_requests_.pop();
        main_logger.debug("Picked a new socket from queue");

        lock.unlock();

        response_writer writer(socket);

        request_parser request;
        try {
            request.parse(socket);
        }
        catch(std::exception& e) {
            std::string msg = "Error parsing request: " +
                    std::string(e.what());
            main_logger.error(msg);
            http::response<http::string_body> response{
                http::status::internal_server_error, 1};
            response.body() = msg;
            writer.write(std::move(response));
            socket.shutdown(socket_type::shutdown_send);
            continue;
        }

        boost::beast::http::verb verb = request.verb();
        main_logger.debug("Received request: " + std::string(request));
        main_logger.debug("Body size: " + std::to_string(
                request.stream().str().size()));

        router::handler_type handler;
        try {
            route requested_route{request.verb_string(), request.target()};
            handler = router_.get_handler(requested_route);
            route mapped_route = router_.get_route(requested_route);
            request.mapped_route(mapped_route);
            handler(request, writer);
        }
        catch(std::out_of_range& e) {
            std::string msg = "Requested target cannot be found: [" +
                    request.verb_string() + "] " + request.target();
            main_logger.error(msg);
            http::response<http::string_body> response{
                    http::status::not_found, 1};
            response.body() = msg;
            writer.write(std::move(response));
        }
        catch(std::exception& e) {
            main_logger.error("Error when handling request [" +
                    request.verb_string() + "] " + request.target() +
                    " : " + std::string{e.what()});
            http::response<http::string_body> response{
                    http::status::internal_server_error, 1};
            response.body() = e.what();
            writer.write(std::move(response));
        }

        socket.shutdown(socket_type::shutdown_send);
    }
}

inline void server::set_router(router& r) {
    router_ = r;
}

} // namespace PROJECT_C
