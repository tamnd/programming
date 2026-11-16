// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-division-by-zero/.

#ifndef SAFE_DIV_H
#define SAFE_DIV_H

#include <stdbool.h>
#include <stdint.h>

// safe_div stores a / b, rounded toward zero, in *out and returns true. It
// returns false when b is 0 or when the quotient does not fit in int64_t.
bool safe_div(int64_t a, int64_t b, int64_t *out);

#endif
