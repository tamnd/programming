# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rotate-a-list/.

def rotate_left(items: list[int], k: int) -> list[int]:
    """Return a new list with the first k items moved to the end."""
    if not items:
        return []
    # k % len(items) makes a k that is larger than the list work.
    s = k % len(items)
    return items[s:] + items[:s]
