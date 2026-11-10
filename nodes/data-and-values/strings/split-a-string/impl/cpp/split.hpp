// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/split-a-string/.

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

// Splits s on the literal separator sep, which must not be empty, and keeps
// the empty fields. With limit n > 0 there are at most n fields, and the last
// field holds the rest of s. A limit of 0 means no limit. The fields are views
// into s, so s must live longer than the result.
std::vector<std::string_view> split(std::string_view s, std::string_view sep, std::size_t limit = 0);

// Splits s on runs of space, tab and LF. There are no empty fields.
std::vector<std::string_view> split_whitespace(std::string_view s);

// Splits s on each match of the ECMAScript regular expression pattern, which
// must not match the empty string. Throws std::regex_error for a bad pattern.
std::vector<std::string> split_regex(const std::string& s, const std::string& pattern);
