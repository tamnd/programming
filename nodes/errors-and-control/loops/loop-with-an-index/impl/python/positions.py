# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-an-index/.

def positions(items: list[str], target: str) -> list[int]:
    """Return the index of each item that equals target."""
    out = []
    # enumerate gives the index and the item together.
    for i, x in enumerate(items):
        if x == target:
            out.append(i)
    return out
