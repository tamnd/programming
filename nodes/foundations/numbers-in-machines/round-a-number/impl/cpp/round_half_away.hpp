// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

#pragma once

#include <cmath>

// Rounds x to the nearest integer; a half goes away from zero.
inline long long round_half_away(double x) {
    // std::llround rounds a half away from zero in each rounding mode.
    return std::llround(x);
}
