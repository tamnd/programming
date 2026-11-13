# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

from collections import Counter


def count_items(items: list[int]) -> list[tuple[int, int]]:
    """Return one (item, count) pair for each distinct item, in ascending order of the item."""
    return sorted(Counter(items).items())
