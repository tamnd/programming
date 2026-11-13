# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-a-list/.

def keep_odd(items: list[int]) -> list[int]:
    """Return a new list of the odd items of items, in their order."""
    # x % 2 != 0 is true for each odd item, also for -3.
    return [x for x in items if x % 2 != 0]
