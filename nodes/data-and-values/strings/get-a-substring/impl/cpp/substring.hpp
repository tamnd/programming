// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#pragma once

#include <cstddef>
#include <string>
#include <string_view>

// substring returns the code points of the UTF-8 string s from index start up
// to, not including, end.
std::string substring(std::string_view s, std::size_t start, std::size_t end);
