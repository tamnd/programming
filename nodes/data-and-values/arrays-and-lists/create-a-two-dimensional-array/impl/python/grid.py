# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

def grid(rows: int, cols: int, value: int) -> list[list[int]]:
    """Return rows lists of cols items, each item value. No two rows share a list."""
    # The comprehension makes a new row for each row. [[value] * cols] * rows
    # would repeat one row object.
    return [[value] * cols for _ in range(rows)]
