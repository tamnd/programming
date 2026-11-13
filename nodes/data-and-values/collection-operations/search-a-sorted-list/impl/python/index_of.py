# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-a-sorted-list/.

from bisect import bisect_left


def index_of(items: list[int], target: int) -> int:
    """Return the first index of target in the sorted list items, or -1."""
    # bisect_left gives the first index i with items[i] >= target.
    i = bisect_left(items, target)
    if i < len(items) and items[i] == target:
        return i
    return -1
