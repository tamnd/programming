# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-two-lists/.

def concat(a: list[int], b: list[int]) -> list[int]:
    """Return a new list with the items of a, then the items of b."""
    # + makes a new list. a += b would change a in place.
    return a + b
