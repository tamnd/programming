# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-an-array/.

def filled(n: int, value: int) -> list[int]:
    """Return a new list of n items, and each item is value."""
    # Repetition copies the reference to value. An int cannot change, so the
    # shared reference does no harm here.
    return [value] * n
