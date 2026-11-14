# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-index-of-a-value/.

def index_of(items: list[int], value: int) -> int:
    """Return the first index of value in items, or -1 if it is not there."""
    try:
        return items.index(value)
    except ValueError:
        # list.index raises an error, not -1, when the value is not there.
        return -1
