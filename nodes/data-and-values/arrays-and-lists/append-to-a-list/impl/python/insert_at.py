# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-list/.

def insert_at(items: list[int], index: int, value: int) -> list[int]:
    """Return a new list with value at position index. index len(items) appends."""
    if not 0 <= index <= len(items):
        # list.insert would not fail here: it clamps the index.
        raise IndexError(index)
    out = list(items)
    out.insert(index, value)
    return out
