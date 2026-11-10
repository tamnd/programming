// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-an-integer/.

#pragma once

#include <cstdint>
#include <expected>
#include <string_view>
#include <system_error>

// parse_integer parses s as a signed 64-bit integer in the given base, from 2
// to 36. The error is std::errc::invalid_argument when s is not an optional
// sign and ASCII digits of the base, and std::errc::result_out_of_range when
// the value does not fit in std::int64_t.
std::expected<std::int64_t, std::errc> parse_integer(std::string_view s, int base);
