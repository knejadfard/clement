#pragma once

#include <boost/algorithm/string/predicate.hpp>
#include <ostream>
#include <string>
#include <vector>

namespace clement {

    class path {

      public:
        using container_type = std::vector<std::string>;
        using iterator_type = container_type::const_iterator;

      private:
        container_type fragments_;

        // parse a string and add its fragments to the vector
        void parse_(const std::string& str) {
            std::size_t prev = 0;
            std::size_t pos = 0;
            while (prev < str.length()) {
                std::string part;
                pos = str.find('/', prev);
                if (pos == std::string::npos) {
                    // deal with last part of the path
                    part = str.substr(prev, str.length() - prev);
                    fragments_.push_back(part);
                    break;
                } else {
                    // read until the next '/' character
                    part = str.substr(prev, pos - prev);
                    if (part.length() > 0) {
                        fragments_.push_back(part);
                    }
                    prev = pos + 1;
                }
            }
        }

      public:
        path(std::string const& str) : fragments_{} { parse_(str); }

        path(char const* str) : fragments_{} { parse_(str); }

        path() : fragments_{} {}

        path& operator/(path const& p) {
            container_type fragments = p.get_fragments();
            for (std::string fragment : fragments) {
                fragments_.push_back(fragment);
            }
            return *this;
        }

        path& operator/(std::string const& fragment) {
            fragments_.push_back(fragment);
            return *this;
        }

        void set(std::string const& p) { parse_(p); }

        std::string get_string() const {
            std::string result{"/"};
            bool is_last_element = false;
            for (std::vector<std::string>::const_iterator it = fragments_.cbegin();
                 it != fragments_.cend(); ++it) {
                result += *it;
                if (it < fragments_.end() - 1) {
                    result += '/';
                }
            }
            return result;
        }

        operator std::string() const { return get_string(); }

        container_type get_fragments() const { return fragments_; }

        friend bool operator==(path const&, path const&);
        friend bool operator!=(path const&, path const&);
        friend std::ostream& operator<<(std::ostream&, path const&);
    };

    bool operator==(path const& lhs, path const& rhs) {
        path::container_type v_lhs = lhs.get_fragments();
        path::container_type v_rhs = rhs.get_fragments();

        if (v_lhs.size() != v_rhs.size()) {
            return false;
        }

        for (path::iterator_type it1 = v_lhs.begin(), it2 = v_rhs.begin();
             it1 != v_lhs.end() && it2 != v_rhs.end(); ++it1, ++it2) {
            // if either one of the fragments starts with ":", then the literal
            // value of these fragments should not be compared (path param name)
            if (boost::starts_with(*it1, ":") || boost::starts_with(*it2, ":")) {
                continue;
            } else if (*it1 != *it2) {
                return false;
            }
        }
        return true;

        /*
        for(int i = 0; i < v_lhs.size(); ++i) {
            // if either one of the fragments start with ':' (path param), while the
            // other fragment does not, then the two routes are not equivalent.
            bool lhs_fragment_param = boost::starts_with(v_lhs.at(i), ":");
            bool rhs_fragment_param = boost::starts_with(v_rhs.at(i), ":");

            if((lhs_fragment_param && !rhs_fragment_param) ||
                    (!lhs_fragment_param && rhs_fragment_param)) {
                return false;
            }
            // if both fragments are path parameters, then the parameter names
            // should not be compared for equality. Just the fact that both are
            // path paramethers means that these fragments are equivalent.
            else if(lhs_fragment_param && rhs_fragment_param) {
                continue;
            }
            else if(v_lhs.at(i) != v_rhs.at(i)) {
                return false;
            }
        }
        return true;
        */
    }

    bool operator!=(path const& lhs, path const& rhs) { return !(lhs == rhs); }

    std::ostream& operator<<(std::ostream& out, path const& p) {
        out << p.get_string();
        return out;
    }

} // namespace clement
