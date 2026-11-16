# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

import functools


def fibonacci(n: int) -> int:
    """Return F(n) with a loop that keeps the last two numbers."""
    if n < 0:
        raise ValueError("n is negative")
    if n == 0:
        return 0
    a, b = 0, 1
    for _ in range(n - 1):
        a, b = b, a + b
    return b


@functools.cache
def fibonacci_cached(n: int) -> int:
    """Return F(n) with recursion and a cache. Each step uses one stack frame."""
    return n if n < 2 else fibonacci_cached(n - 1) + fibonacci_cached(n - 2)
