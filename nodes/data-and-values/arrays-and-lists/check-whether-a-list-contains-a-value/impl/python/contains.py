# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

def contains(items: list[int], value: int) -> bool:
    """Return True if some item of items is equal to value."""
    # in compares each item with value, so it takes time in proportion to len(items).
    return value in items
