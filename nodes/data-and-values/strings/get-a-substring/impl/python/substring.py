# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-substring/.


def substring(s: str, start: int, end: int) -> str:
    """Return the code points of s from index start up to, not including, end."""
    # A str is a sequence of code points, so a slice cuts at code points.
    return s[start:end]
