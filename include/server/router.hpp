#pragma once

#include "logger.hpp"
#include "request_parser.hpp"
#include "route.hpp"
#include "writer.hpp"
#include <boost/beast/http.hpp>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace server {

    template <class Stream> class basic_router {

      public:
        using handler_type = std::function<void(request_parser&, basic_writer<Stream>&)>;
        using element_type = std::pair<route, handler_type>;
        using container_type = std::vector<element_type>;
        using iterator_type = typename container_type::iterator;
        using const_iterator_type = typename container_type::const_iterator;

      private:
        container_type routes_;

      public:
        basic_router() : routes_{} {}

        basic_router(std::initializer_list<element_type> routes) : routes_{routes} {}

        void add(element_type r) { routes_.push_back(r); }

        void get(std::string const& r, handler_type h) {
            element_type item{route{"GET", r}, h};
            routes_.push_back(item);
        }

        void post(std::string const& r, handler_type h) {
            element_type item{route{"POST", r}, h};
            routes_.push_back(item);
        }

        void put(std::string const& r, handler_type h) {
            element_type item{route{"PUT", r}, h};
            routes_.push_back(item);
        }

        void del(std::string const& r, handler_type h) {
            element_type item{route{"DEL", r}, h};
            routes_.push_back(item);
        }

        container_type get_routes() const { return routes_; }

        handler_type get_handler(route const& r) {
            main_logger.debug("Attempting to find a handler for route: " + std::string{r});
            for (iterator_type it = routes_.begin(); it != routes_.end(); ++it) {
                if (it->first == r) {
                    return it->second;
                }
            }
            throw std::runtime_error("could not map '" + std::string{r} +
                                     "' to any handlers in the router");
        }

        route get_route(route const& r) {
            main_logger.debug("Attempting to find a route for: " + std::string{r});
            for (iterator_type it = routes_.begin(); it != routes_.end(); ++it) {
                if (it->first == r) {
                    return it->first;
                }
            }
            throw std::runtime_error("could not map '" + std::string{r} +
                                     "' to any routes in the router");
        }
    };

    using router = basic_router<boost::asio::ip::tcp::socket>;

} // namespace server
