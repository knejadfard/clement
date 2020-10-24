#ifndef CLEMENT_RESPONSE_BUILDER_H
#define CLEMENT_RESPONSE_BUILDER_H

#include <boost/beast/http.hpp>
#include <string>

namespace clement {

    template <class Body> class basic_response_builder {

      public:
        using response_type = boost::beast::http::response<Body>;

      private:
        response_type response_;
        bool prepare_ = true;
        bool allow_all_origins_ = true;

      public:
        basic_response_builder() : response_{} {}

        basic_response_builder(basic_response_builder&&) = delete;
        basic_response_builder(basic_response_builder const&) = delete;
        basic_response_builder& operator=(basic_response_builder const&) = delete;
        basic_response_builder& operator=(basic_response_builder const&&) = delete;

        void header(std::string const& name, std::string const& value) {
            response_.set(name, value);
        }

        void result(boost::beast::http::status const& status) { response_.result(status); }

        void body(std::string const& body) { response_.body() = body; }

        void body(std::string const&& body) { response_.body() = body; }

        void prepare(bool const& b) { prepare_ = b; }

        boost::beast::http::response<Body> build() {
            if (allow_all_origins_) {
                header("access-control-allow-origin", "*");
            }
            if (prepare_) {
                response_.prepare_payload();
            }
            return response_;
        }
    };

    using response_builder = basic_response_builder<boost::beast::http::string_body>;

} // namespace clement

#endif