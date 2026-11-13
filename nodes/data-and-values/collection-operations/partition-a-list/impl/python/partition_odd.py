# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/partition-a-list/.

def partition_odd(items: list[int]) -> tuple[list[int], list[int]]:
    """Return the odd items and the even items of items, each in input order."""
    odd: list[int] = []
    even: list[int] = []
    for x in items:
        # x % 2 != 0 is true for each odd item, also for -3.
        (odd if x % 2 != 0 else even).append(x)
    return odd, even
