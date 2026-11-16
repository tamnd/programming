// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#include "cycle.hpp"

#include <array>

namespace {
constexpr std::array<std::string_view, day_count> names{"monday", "tuesday", "wednesday", "thursday",
                                                        "friday", "saturday", "sunday"};
}

std::string_view to_string(Day d) { return names.at(static_cast<std::size_t>(d)); }

std::optional<Day> parse_day(std::string_view s) {
    for (int i = 0; i < day_count; i++) {
        if (names[static_cast<std::size_t>(i)] == s) {
            return static_cast<Day>(i);
        }
    }
    return std::nullopt;
}

std::vector<std::string> cycle(std::string_view start, int steps) {
    std::vector<std::string> out;
    auto d = parse_day(start);
    if (!d) {
        return out;
    }
    int i = static_cast<int>(*d);
    for (int k = 0; k < steps; k++) {
        out.emplace_back(to_string(static_cast<Day>((i + k) % day_count)));
    }
    return out;
}
