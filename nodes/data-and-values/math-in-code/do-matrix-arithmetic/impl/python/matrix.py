# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-matrix-arithmetic/.


def transpose(m: list[list[int]]) -> list[list[int]]:
    """Return the transpose of m: row i of the result is column i of m."""
    return [list(col) for col in zip(*m)]


def matrix_multiply(a: list[list[int]], b: list[list[int]]) -> list[list[int]]:
    """Return the product a times b.

    The number of columns of a must be the number of rows of b. Neither
    matrix changes.
    """
    if len(a[0]) != len(b):
        raise ValueError(f"{len(a)}x{len(a[0])} times {len(b)}x{len(b[0])}: the sizes do not match")
    cols = list(zip(*b))
    return [[sum(x * y for x, y in zip(row, col)) for col in cols] for row in a]
