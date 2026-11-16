# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pattern-match-on-a-value/.

def classify(x: int, y: int) -> str:
    """Name the place of the point (x, y): origin, x-axis, y-axis, diagonal or other."""
    match (x, y):
        case (0, 0):
            return "origin"
        case (_, 0):
            return "x-axis"
        case (0, _):
            return "y-axis"
        case (a, b) if a == b:
            return "diagonal"
        case _:
            return "other"
