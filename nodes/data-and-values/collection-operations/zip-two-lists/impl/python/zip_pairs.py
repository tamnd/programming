# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/zip-two-lists/.

def zip_pairs(a: list[int], b: list[int]) -> list[tuple[int, int]]:
    """Return the pairs (a[i], b[i]) up to the end of the shorter list."""
    return list(zip(a, b))
