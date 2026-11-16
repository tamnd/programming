// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/round-a-number/.

#include "round_half_away.h"

#include <math.h>

long long round_half_away(double x) {
    // llround rounds a half away from zero in each rounding mode.
    return llround(x);
}
