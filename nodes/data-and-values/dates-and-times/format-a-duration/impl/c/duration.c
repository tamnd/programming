// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-duration/.

#include "duration.h"

#include <stdio.h>

void format_duration(long long seconds, char out[FORMAT_DURATION_SIZE]) {
    // The magnitude is unsigned, so -LLONG_MIN does not overflow.
    const unsigned long long a =
        seconds < 0 ? 0ULL - (unsigned long long)seconds : (unsigned long long)seconds;
    snprintf(out, FORMAT_DURATION_SIZE, "%s%02llu:%02llu:%02llu", seconds < 0 ? "-" : "",
             a / 3600, a % 3600 / 60, a % 60);
}
