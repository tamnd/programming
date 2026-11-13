# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/map-over-a-list/.

def square_all(items: list[int]) -> list[int]:
    """Return a new list with the square of each item, in the same order."""
    # A list comprehension makes a list. map(...) makes a one-pass iterator.
    return [x * x for x in items]
