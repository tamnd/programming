// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#pragma once

#include <string_view>

// balanced returns true if the brackets of text match and nest correctly.
bool balanced(std::string_view text);
