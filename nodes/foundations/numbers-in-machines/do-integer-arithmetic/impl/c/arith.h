// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

#ifndef ARITH_H
#define ARITH_H

#include <stdint.h>

// arith stores six results in out: a + b, a - b, a * b, a / b (rounded
// toward zero), a % b (with the sign of a) and a to the power n. b must not
// be 0.
void arith(int64_t a, int64_t b, int n, int64_t out[6]);

#endif
