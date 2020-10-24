#ifndef CLEMENT_REQUEST_HPP
#define CLEMENT_REQUEST_HPP

#include "path.hpp"
#include "route.hpp"
#include "utility.hpp"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <map>
#include <sstream>
#include <string>

namespace clement {

    namespace http = boost::beast::http;

    class request {

        boost::beast::http::verb verb_;
        std::string target_;
        std::map<boost::beast::http::field, std::string> headers_;
        std::map<std::string, std::string> query_params_;
        std::map<std::string, std::string> path_params_;
        std::stringstream stream_;
        std::string content_type_;
        route mapped_route_;

        template <class Body> void read_fields(boost::beast::http::request<Body>&);

        // TODO this can simply read from headers_, since we have already parsed them
        template <class Body> void parse_target(boost::beast::http::request<Body>&);

        // TODO this can simply read from headers_, since we have already parsed them
        template <class Body> void parse_content_type(boost::beast::http::request<Body>&);

        void parse_path_params();

        // TODO is an instance of request copy-able and/or move-able? Might need to disable these.

      public:
        request() {}

        void parse_header(boost::beast::http::request<boost::beast::http::empty_body>&);

        void read_body(boost::beast::http::request<boost::beast::http::dynamic_body>&);

        void header(boost::beast::http::field const& name, std::string const& value) {
            headers_[name] = value;
        }

        std::string header(boost::beast::http::field const& name) const {
            std::map<boost::beast::http::field, std::string>::const_iterator it =
                headers_.find(name);
            if (it == headers_.end()) {
                return std::string();
            }
            return it->second;
        }

        std::string header(std::string const& name) const {
            boost::beast::http::field f = boost::beast::http::string_to_field(name);
            return header(f);
        }

        std::map<boost::beast::http::field, std::string> headers() const { return headers_; }

        boost::beast::http::verb verb() const { return verb_; }

        std::string verb_string() const { return to_string(verb_).to_string(); }

        std::string target() const { return target_; }

        std::stringstream& stream() { return stream_; }

        std::map<std::string, std::string> query_params() const { return query_params_; }

        std::map<std::string, std::string> path_params() const { return path_params_; }

        explicit operator std::string() const { return "[" + verb_string() + "] " + target_; }

        std::string content_type() const { return content_type_; }

        void mapped_route(route const& r) {
            mapped_route_ = r;
            parse_path_params();
        }

        route mapped_route() const { return mapped_route_; }
    };

    void request::parse_header(boost::beast::http::request<boost::beast::http::empty_body>& request_header) {
        read_fields(request_header);
        parse_target(request_header);
        verb_ = request_header.method();
        parse_content_type(request_header);
    }

    void request::read_body(boost::beast::http::request<boost::beast::http::dynamic_body>& request_body) {
        boost::beast::multi_buffer body_buffer = request_body.body();
        // TODO what if the request body should not be treated as text/string?
        // TODO performance concerns: what if the body is too large? Needs performance tests.
        stream_ << boost::beast::make_printable(body_buffer.cdata());
    }

    template <class Body>
    void request::read_fields(boost::beast::http::request<Body>& request_header) {
        for (typename boost::beast::http::request<Body>::const_iterator it = request_header.begin();
             it != request_header.end(); ++it) {
            header(it->name(), it->value().to_string());
        }
    }

    // TODO would it be more robust/cleaner to use boost's tokenizer and related
    // methods for parsing the target string?
    template <class Body>
    void request::parse_target(boost::beast::http::request<Body>& request_header) {
        std::string unparsed_target = request_header.target().to_string();
        size_t qmark_index = unparsed_target.find("?");
        if (qmark_index == std::string::npos) {
            // no query parameters are present, assume everything is target string
            target_ = clement::utility::percent_decode(unparsed_target);
        } else {
            target_ = clement::utility::percent_decode(unparsed_target.substr(0, qmark_index));
            if (qmark_index != unparsed_target.size() - 1) {
                std::string raw_query_params = unparsed_target.substr(qmark_index + 1);
                size_t begin = 0, end;
                bool done = false;
                while (!done) {
                    end = raw_query_params.find("&", begin + 1);
                    done = (end == std::string::npos);
                    std::string query_param = raw_query_params.substr(begin, end - begin);
                    size_t equal_sign_index = query_param.find("=");
                    if (equal_sign_index == std::string::npos) {
                        throw std::runtime_error("Error parsing target string: " + unparsed_target);
                    }
                    std::string param_name =
                        clement::utility::percent_decode(query_param.substr(0, equal_sign_index));
                    std::string param_value =
                        clement::utility::percent_decode(
                        query_param.substr(equal_sign_index + 1));
                    query_params_[param_name] = param_value;
                    begin = end + 1;
                }
            }
        }
    }

    template <class Body>
    void request::parse_content_type(boost::beast::http::request<Body>& request_header) {
        std::string raw_content_type = header("content-type");
        size_t end = raw_content_type.find(";");
        if (end == std::string::npos) {
            content_type_ = raw_content_type;
        } else {
            content_type_ = raw_content_type.substr(0, end);
        }
    }

    void request::parse_path_params() {
        path requested_path{target_};
        path mapped_path = mapped_route_.get_path();
        path::container_type requested_frags = requested_path.get_fragments();
        path::container_type mapped_frags = mapped_path.get_fragments();

        if (requested_frags.size() != mapped_frags.size()) {
            throw std::runtime_error("the size of requested and mapped routes do not match: <" +
                                     std::string{requested_path} + ">, <" +
                                     std::string{mapped_path} + ">");
        }

        for (path::iterator_type it1 = mapped_frags.cbegin(), it2 = requested_frags.cbegin();
             it1 != mapped_frags.cend() && it2 != mapped_frags.cend(); ++it1, ++it2) {
            if (boost::starts_with(*it1, ":")) {
                std::string param_name = it1->substr(1);
                path_params_[param_name] = *it2;
                main_logger.debug("Adding path parameter: " + param_name + "=" + *it2);
            }
        }
    }

} // namespace clement

#endif
