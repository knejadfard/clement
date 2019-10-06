#pragma once

#include "logger.hpp"
#include "path.hpp"
#include <boost/beast/http/verb.hpp>
#include <ostream>
#include <string>

namespace PROJECT_C {

class route {

  boost::beast::http::verb verb_;
  path path_;

public:
  route() : verb_{}, path_{} {}

  route(const std::string& verb, const path& p)
      : verb_{boost::beast::http::string_to_verb(verb)}, path_{p} {}

  route(boost::beast::http::verb verb, const path& p) : verb_{verb}, path_{p} {}

  route(route const& rhs) : verb_{rhs.verb()}, path_{rhs.get_path()} {}

  route& operator=(route const& rhs) {
    verb_ = rhs.verb_;
    path_ = rhs.path_;
    return *this;
  }

  boost::beast::http::verb verb() const { return verb_; }

  std::string verb_string() const { return boost::beast::http::to_string(verb_).to_string(); }

  path get_path() const { return path_; }

  operator std::string() const {
    std::string result = "[" + verb_string() + "] " + path_.get_string();
    return result;
  }

  friend std::ostream& operator<<(std::ostream&, route const&);
  friend bool operator<(route const&, route const&);
  friend bool operator==(route const&, route const&);
  friend bool operator!=(route const&, route const&);
};

std::ostream& operator<<(std::ostream& out, const route& r) {
  out << std::string(r);
  return out;
}

bool operator==(const route& lhs, const route& rhs) {
  if (lhs.verb_string() != rhs.verb_string()) {
    return false;
  }
  return lhs.get_path() == rhs.get_path();
}

bool operator!=(route const& lhs, route const& rhs) { return !(lhs == rhs); }

} // namespace PROJECT_C
