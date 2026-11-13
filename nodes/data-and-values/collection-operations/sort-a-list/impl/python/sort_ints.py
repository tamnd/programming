# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list/.

def sort_ints(items: list[int]) -> list[int]:
    """Return a new list of the items in ascending order."""
    # sorted makes a new list. items.sort() sorts in place and returns None.
    return sorted(items)
