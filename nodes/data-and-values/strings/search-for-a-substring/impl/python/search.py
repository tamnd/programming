# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-for-a-substring/.


def find(s: str, sub: str) -> int:
    """Return the code point index of the first match of sub in s, or -1."""
    # A str is a sequence of code points, so str.find counts code points.
    return s.find(sub)


def find_last(s: str, sub: str) -> int:
    """Return the code point index of the last match of sub in s, or -1."""
    return s.rfind(sub)
