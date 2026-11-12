// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#include "concatenate.hpp"

#include <format>

std::string concat(std::string_view a, std::string_view b) {
    // operator+ takes std::string operands, so the code builds one string
    // with room for both parts.
    std::string r;
    r.reserve(a.size() + b.size());
    r += a;
    r += b;
    return r;
}

std::string concat_number(std::string_view s, std::int32_t n) {
    // std::format writes the decimal digits without a locale.
    return std::format("{}{}", s, n);
}
