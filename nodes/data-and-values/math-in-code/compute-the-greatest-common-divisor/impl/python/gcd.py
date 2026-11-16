# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

import math


def gcd_and_lcm(a: int, b: int) -> tuple[int, int]:
    """Return the greatest common divisor and the least common multiple."""
    return math.gcd(a, b), math.lcm(a, b)


def gcd_and_lcm_euclid(a: int, b: int) -> tuple[int, int]:
    """Return the same pair with Euclid's algorithm."""
    x, y = abs(a), abs(b)
    while y != 0:
        x, y = y, x % y
    lcm = 0 if a == 0 or b == 0 else abs(a) // x * abs(b)
    return x, lcm
