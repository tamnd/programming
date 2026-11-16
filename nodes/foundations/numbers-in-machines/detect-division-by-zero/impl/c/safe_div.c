// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

#include "safe_div.h"

bool safe_div(int64_t a, int64_t b, int64_t *out) {
    // Both cases are undefined behavior in C, so test them before the
    // division.
    if (b == 0 || (a == INT64_MIN && b == -1)) {
        return false;
    }
    *out = a / b; // C rounds the quotient toward zero
    return true;
}
