// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

#include "manhattan.h"

#include <stdlib.h>

int manhattan(const int p[2], const int q[2]) {
    // C has no destructuring, so each name gets one element by its index.
    int x1 = p[0], y1 = p[1];
    int x2 = q[0], y2 = q[1];
    return abs(x1 - x2) + abs(y1 - y2);
}
