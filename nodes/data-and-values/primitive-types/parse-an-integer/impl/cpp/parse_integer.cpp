// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#include "parse_integer.hpp"

#include <charconv>

std::expected<std::int64_t, std::errc> parse_integer(std::string_view s, int base) {
    // std::from_chars accepts a minus sign but not a plus sign.
    if (s.size() >= 2 && s[0] == '+' && s[1] != '+' && s[1] != '-') {
        s.remove_prefix(1);
    }
    const char *end = s.data() + s.size();
    std::int64_t value = 0;
    auto [ptr, ec] = std::from_chars(s.data(), end, value, base);
    // from_chars stops at the first character that is not a digit, so check
    // that it read the whole string.
    if (ec == std::errc::invalid_argument || ptr != end) {
        return std::unexpected(std::errc::invalid_argument);
    }
    if (ec == std::errc::result_out_of_range) {
        return std::unexpected(ec);
    }
    return value;
}
