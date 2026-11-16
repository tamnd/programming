# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-higher-order-function/.

from collections.abc import Callable


def repeat(f: Callable[[int], int], times: int) -> Callable[[int], int]:
    """Return a function that applies f to its argument times times."""

    def repeated(x: int) -> int:
        for _ in range(times):
            x = f(x)
        return x

    return repeated


def collatz_step(n: int) -> int:
    """Return n / 2 for an even n and 3n + 1 for an odd n."""
    return n // 2 if n % 2 == 0 else 3 * n + 1


def collatz_after(start: int, times: int) -> int:
    """Return the value after times Collatz steps from start."""
    return repeat(collatz_step, times)(start)
