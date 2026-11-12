# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-strings/.


def compare(a: str, b: str) -> int:
    """Return -1, 0 or 1 for the code point order of a and b."""
    # Python compares strings by code points, and True - False is 1.
    return (a > b) - (a < b)


def equals(a: str, b: str) -> bool:
    """Return True if a and b have the same code points."""
    return a == b
