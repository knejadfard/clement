#pragma once

#include "url_utility.hpp"
#include <boost/tokenizer.hpp>
#include <map>
#include <string>

namespace PROJECT_C {

namespace utility {

class form_urlencoded {

public:
  using container_type = std::map<std::string, std::string>;

private:
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

  container_type params_;

  // param is a "key=value" string
  void add_parameter(std::string const& param) {
    boost::char_separator<char> separator{"="};
    tokenizer tok{param, separator};
    tokenizer::iterator it = tok.begin();
    std::string param_name = percent_decode(*it++);
    if (it == tok.end()) {
      throw std::runtime_error("error parsing parameter string: " + param);
    }
    params_[param_name] = percent_decode(*it);
  }

public:
  form_urlencoded() : params_{} {}

  form_urlencoded(std::string const& str) : params_{} { parse(str); }

  void parse(std::string const& str) {
    boost::char_separator<char> separator{"&"};
    tokenizer tok{str, separator};
    for (std::string parameter : tok) {
      add_parameter(parameter);
    }
  }

  container_type params() const { return params_; }
};

} // namespace utility

} // namespace PROJECT_C
