// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

#include "color.hpp"

#include <cstdio>
#include <sstream>
#include <stdexcept>

namespace color {

Color::Color(int r, int g, int b) {
    for (int v : {r, g, b}) {
        if (v < 0 || v > 255) {
            throw std::out_of_range("not a channel value: " + std::to_string(v));
        }
    }
    r_ = static_cast<std::uint8_t>(r);
    g_ = static_cast<std::uint8_t>(g);
    b_ = static_cast<std::uint8_t>(b);
}

std::string Color::to_string() const {
    char buf[8];
    // A std::uint8_t is a character type, so convert it to int before it is printed as a number.
    std::snprintf(buf, sizeof buf, "#%02X%02X%02X", static_cast<unsigned>(r_), static_cast<unsigned>(g_),
                  static_cast<unsigned>(b_));
    return buf;
}

std::ostream& operator<<(std::ostream& os, const Color& c) { return os << c.to_string(); }

std::vector<std::string> format_colors(const std::vector<int>& reds, const std::vector<int>& greens,
                                       const std::vector<int>& blues) {
    if (reds.size() != greens.size() || reds.size() != blues.size()) {
        throw std::invalid_argument("the lists must have the same length");
    }
    std::vector<std::string> out;
    out.reserve(reds.size());
    for (std::size_t i = 0; i < reds.size(); i++) {
        std::ostringstream s;
        s << Color(reds[i], greens[i], blues[i]);
        out.push_back(s.str());
    }
    return out;
}

}  // namespace color
