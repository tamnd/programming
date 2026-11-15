# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-a-collection/.

def total(items: list[int]) -> int:
    """Return the sum of the items, using a for-each loop."""
    s = 0
    for x in items:
        s += x
    return s
