# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-list/.

def reverse_list(items: list[int]) -> list[int]:
    """Return a new list of the items in the opposite order."""
    # items.reverse() would change items and return None.
    return items[::-1]
