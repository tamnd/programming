# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-nested-loops/.

def find_in_grid(grid: list[list[int]], target: int) -> tuple[int, int]:
    """Return (row, column) of the first cell that equals target, or (-1, -1)."""
    for r, row in enumerate(grid):
        for c, x in enumerate(row):
            if x == target:
                # return leaves both loops. break would leave only the inner one.
                return (r, c)
    return (-1, -1)
