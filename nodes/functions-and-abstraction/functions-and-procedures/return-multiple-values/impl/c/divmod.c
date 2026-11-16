// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/return-multiple-values/.

#include "divmod.h"

void div_mod(int a, int b, int *quotient, int *remainder) {
    // The C operators / and % truncate toward zero. Move a result that is
    // not exact one step down, so that the quotient is the floor.
    int q = a / b;
    int r = a % b;
    if (r != 0 && (r < 0) != (b < 0)) {
        q -= 1;
        r += b;
    }
    *quotient = q;
    *remainder = r;
}
