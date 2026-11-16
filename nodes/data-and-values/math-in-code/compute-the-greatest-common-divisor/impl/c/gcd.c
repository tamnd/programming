// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

#include "gcd.h"

#include <stdlib.h>

void gcd_and_lcm(long long a, long long b, long long *gcd, long long *lcm) {
    // The remainder in C has the sign of the dividend, so the loop works on
    // the absolute values.
    long long x = llabs(a);
    long long y = llabs(b);
    while (y != 0) {
        long long r = x % y;
        x = y;
        y = r;
    }
    *gcd = x;
    *lcm = a == 0 || b == 0 ? 0 : llabs(a) / x * llabs(b);
}
