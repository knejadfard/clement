#pragma once

#include <string>
#include <random>

namespace PROJECT_C {

namespace utility {

std::string
generate_string(size_t const& length) {
    std::string chars{"abcdefghijklmnopqrstuvwxyz1234567890"};
    std::random_device rd;
    std::mt19937 rng{rd()};
    std::uniform_int_distribution<std::mt19937::result_type> index_dist(0, chars.size() - 1);
    std::string result;
    for(int i = 0; i < length; ++i) {
        result.push_back(chars[index_dist(rng)]);
    }
    return result;
}

} // namespace utility

} // namespace PROJECT_C
