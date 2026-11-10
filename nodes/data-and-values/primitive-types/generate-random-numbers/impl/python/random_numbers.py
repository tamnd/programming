# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/generate-random-numbers/.
"""Random integers in a range, a shuffle, a seeded generator and tokens."""

import random
import secrets
from collections.abc import Callable, MutableSequence

type WordSource = Callable[[], int]
"""A function that returns a random integer from 0 to 2**32 - 1."""


def int_in_range(next_word: WordSource, lo: int, hi: int) -> int:
    """Return an integer from lo to hi, each with equal probability.

    A word below 2**32 % n is rejected, so that each remainder w % n
    comes from the same number of words. In real code, random.randint
    and secrets.randbelow do this work.
    """
    n = hi - lo + 1
    if not 1 <= n <= 1 << 32:
        raise ValueError("need lo <= hi and at most 2**32 values")
    least = (1 << 32) % n
    while True:
        word = next_word()
        if word >= least:
            return lo + word % n


def shuffle[T](next_word: WordSource, items: MutableSequence[T]) -> None:
    """Shuffle items in place with the Fisher-Yates method."""
    for i in range(len(items) - 1, 0, -1):
        j = int_in_range(next_word, 0, i)
        items[i], items[j] = items[j], items[i]


def seeded_words(seed: int) -> WordSource:
    """Return a word source that gives the same words for the same seed."""
    rng = random.Random(seed)
    return lambda: rng.getrandbits(32)


def secure_words() -> WordSource:
    """Return a word source from the secure generator of the system."""
    return lambda: secrets.randbits(32)


def token_hex(nbytes: int) -> str:
    """Return nbytes secure random bytes as 2 * nbytes hexadecimal digits."""
    return secrets.token_hex(nbytes)
