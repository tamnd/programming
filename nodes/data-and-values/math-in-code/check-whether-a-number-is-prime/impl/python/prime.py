# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

import math


def is_prime(n: int) -> bool:
    """Return True if n is prime, with trial division up to the square root."""
    if n < 2:
        return False
    if n % 2 == 0:
        return n == 2
    for i in range(3, math.isqrt(n) + 1, 2):
        if n % i == 0:
            return False
    return True
