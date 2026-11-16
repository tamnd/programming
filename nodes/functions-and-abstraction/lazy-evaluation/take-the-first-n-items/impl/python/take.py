# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/take-the-first-n-items/.

from itertools import count, islice


def take(items: list[int], n: int) -> list[int]:
    """Return a copy of the first n items; a shorter list gives all its items."""
    # A slice stops at the end of the list, so n can be larger than len(items).
    return items[:n]


def take_naturals(start: int, n: int) -> list[int]:
    """Return the first n items of the infinite sequence start, start + 1, ..."""
    # count is infinite and lazy; islice reads only n items from it.
    return list(islice(count(start), n))
