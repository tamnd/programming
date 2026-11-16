# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-factorial/.

import math


def factorial(n: int) -> int:
    """Return n! with the library function."""
    return math.factorial(n)


def factorial_loop(n: int) -> int:
    """Return n! with a loop."""
    if n < 0:
        raise ValueError("n is negative")
    f = 1
    for i in range(2, n + 1):
        f *= i
    return f


def factorial_recursive(n: int) -> int:
    """Return n! with recursion. Each step uses one stack frame."""
    return 1 if n == 0 else n * factorial_recursive(n - 1)
