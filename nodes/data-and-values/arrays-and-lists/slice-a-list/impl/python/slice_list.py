# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/slice-a-list/.

def slice_list(items: list[int], start: int, stop: int) -> list[int]:
    """Return a new list of items[start] up to, but not including, items[stop]."""
    if not 0 <= start <= stop <= len(items):
        # A slice would not fail here: it clamps the bounds.
        raise IndexError((start, stop))
    return items[start:stop]
