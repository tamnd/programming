// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

#ifndef GCD_AND_LCM_H
#define GCD_AND_LCM_H

// Sets *gcd and *lcm for a and b. Neither a nor b can be LLONG_MIN, and the
// least common multiple must fit in a long long.
void gcd_and_lcm(long long a, long long b, long long *gcd, long long *lcm);

#endif
