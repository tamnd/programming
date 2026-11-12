// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.
#pragma once

#include <string>
#include <string_view>
#include <vector>

// characters returns the code points of the UTF-8 string s, each as a string.
std::vector<std::string> characters(std::string_view s);
