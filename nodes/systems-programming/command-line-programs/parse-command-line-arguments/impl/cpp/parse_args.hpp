// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.
// since: C++23 (std::expected)

#pragma once

#include <expected>
#include <span>
#include <string>
#include <string_view>
#include <vector>

struct CountArgs {
    bool verbose = false;
    int limit = 10;
    std::vector<std::string> files;
    bool operator==(const CountArgs&) const = default;
};

// Parses the arguments of "count [-v] [-n N] [--] [FILE...]" without the
// program name. The error is a message for the user.
std::expected<CountArgs, std::string> parse_args(std::span<const std::string_view> args);
