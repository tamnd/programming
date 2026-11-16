// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

#pragma once

#include <string>

// classify names the place of the point (x, y): origin, x-axis, y-axis,
// diagonal or other.
std::string classify(int x, int y);
