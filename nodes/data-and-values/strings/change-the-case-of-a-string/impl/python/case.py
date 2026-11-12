# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/change-the-case-of-a-string/.

"""Change the case of the ASCII letters of a string."""

_UPPER = str.maketrans("abcdefghijklmnopqrstuvwxyz", "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
_LOWER = str.maketrans("ABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyz")


def to_upper(s: str) -> str:
    """Return s with a to z changed to A to Z.

    str.upper changes each cased character, and "ß".upper() is "SS", so the
    function uses a table of the 26 ASCII letters.
    """
    return s.translate(_UPPER)


def to_lower(s: str) -> str:
    """Return s with A to Z changed to a to z."""
    return s.translate(_LOWER)
