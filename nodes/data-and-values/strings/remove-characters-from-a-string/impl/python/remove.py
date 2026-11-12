# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-characters-from-a-string/.

"""Remove each code point of a set from a string."""


def remove_chars(s: str, chars: str) -> str:
    """Return s without the code points of chars.

    A str is a sequence of code points, and the third argument of
    str.maketrans maps each of its code points to None, so translate
    removes them.
    """
    return s.translate(str.maketrans("", "", chars))
