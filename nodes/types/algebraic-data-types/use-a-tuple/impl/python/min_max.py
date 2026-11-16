# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-tuple/.


def min_max(numbers: list[int]) -> tuple[int, int]:
    """Returns (smallest, largest) of a list with at least one number."""
    lo = hi = numbers[0]
    for n in numbers[1:]:
        if n < lo:
            lo = n
        elif n > hi:
            hi = n
    return lo, hi  # the comma makes the tuple
