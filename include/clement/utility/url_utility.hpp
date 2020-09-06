#pragma once

#include <exception>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <string>

namespace clement {

    namespace utility {

        bool is_unreserved(char const& c) {
            if (std::isalnum(c)) {
                return true;
            } else if (c == '-' || c == '_' || c == '.' || c == '~') {
                return true;
            }
            return false;
        }

        std::string to_hex(unsigned int const& c) {
            std::stringstream stream;
            stream << '%' << std::setw(2) << std::setfill('0') << std::hex << c;
            return stream.str();
        }

        std::string percent_encode(std::string const& s) {
            std::string encoded;
            for (std::string::const_iterator it = s.cbegin(); it < s.cend(); ++it) {
                if (is_unreserved(*it)) {
                    encoded += *it;
                } else if (*it == ' ') {
                    encoded += "%20";
                } else {
                    encoded += to_hex(*it);
                }
            }
            return encoded;
        }

        std::string percent_decode(std::string const& s) {
            std::string decoded;
            for (std::string::const_iterator it = s.cbegin(); it < s.cend(); ++it) {
                if (*it == '%') {
                    std::stringstream stream;
                    // ensure we have at least 2 more characters left
                    if (std::next(it, 2) < s.cend()) {
                        stream << *++it;
                        stream << *++it;
                        if (stream.str() == "20") {
                            decoded += ' ';
                        } else {
                            unsigned int c;
                            stream >> std::hex >> c;
                            decoded += c;
                        }
                    } else {
                        throw std::runtime_error("EXCEPTION");
                    }
                } else {
                    decoded += *it;
                }
            }
            return decoded;
        }

    } // namespace utility

} // namespace clement
