# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/destructure-a-value/.

def manhattan(p: list[int], q: list[int]) -> int:
    """Return the Manhattan distance between the points p = [x, y] and q = [x, y]."""
    x1, y1 = p
    x2, y2 = q
    return abs(x1 - x2) + abs(y1 - y2)
