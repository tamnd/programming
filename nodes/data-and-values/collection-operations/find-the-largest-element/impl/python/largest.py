# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-largest-element/.

def largest(items: list[int]) -> int:
    """Return the largest item of items. The list must not be empty."""
    # max raises ValueError for an empty list.
    return max(items)
