# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-length-of-an-array/.

def length(items: list[str]) -> int:
    """Return the number of items in items."""
    # len works on any sequence or collection, but not on a generator.
    return len(items)
