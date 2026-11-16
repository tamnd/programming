# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-lazy-sequences/.

from collections.abc import Iterator
from itertools import count, islice


def is_prime(k: int) -> bool:
    """Return True if k is a prime number, by trial division."""
    if k < 2:
        return False
    d = 2
    while d * d <= k:
        if k % d == 0:
            return False
        d += 1
    return True


def primes() -> Iterator[int]:
    """Return the infinite lazy sequence of the prime numbers."""
    # count(2) has no end, and filter tests a number only when it is read.
    return filter(is_prime, count(2))


def first_primes(n: int) -> list[int]:
    """Return the first n prime numbers."""
    # islice stops after n items, so the pipeline reads only what it needs.
    return list(islice(primes(), n))
