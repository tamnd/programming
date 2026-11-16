// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

#include "arith.h"

void arith(int64_t a, int64_t b, int n, int64_t out[6]) {
    // C has no power operator for integers. pow from <math.h> works on
    // double values, so a loop keeps the power exact. ^ is the bitwise
    // exclusive or.
    int64_t power = 1;
    for (int i = 0; i < n; i++) {
        power *= a;
    }
    out[0] = a + b;
    out[1] = a - b;
    out[2] = a * b;
    out[3] = a / b;
    out[4] = a % b;
    out[5] = power;
}
