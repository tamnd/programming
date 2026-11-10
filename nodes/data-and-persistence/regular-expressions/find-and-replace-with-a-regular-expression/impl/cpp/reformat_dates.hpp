// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

#pragma once

#include <array>
#include <string>
#include <vector>

// Changes each date YYYY<from_sep>MM<from_sep>DD in text to DD<to_sep>MM<to_sep>YYYY.
std::string reformat_dates(const std::string& text, const std::string& from_sep, const std::string& to_sep);

// Returns {year, month, day} for each date YYYY<sep>MM<sep>DD in text.
std::vector<std::array<std::string, 3>> find_dates(const std::string& text, const std::string& sep);
