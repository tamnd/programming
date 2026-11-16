// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

// A scoped enumeration: the names are Day::Monday and so on, and a Day does
// not convert to int without a static_cast.
enum class Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

inline constexpr int day_count = 7;

std::string_view to_string(Day d);
std::optional<Day> parse_day(std::string_view s);

// cycle returns steps day names from start on, with monday after sunday.
std::vector<std::string> cycle(std::string_view start, int steps);
