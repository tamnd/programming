# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.


def characters(s: str) -> list[str]:
    """Return the code points of s, each as a string of one code point.

    A Python str is a sequence of code points, so iteration gives code points.
    """
    return list(s)
