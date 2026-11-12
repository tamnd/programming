# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

# The six code points of ASCII white space.
ASCII_SPACE = " \t\n\v\f\r"


def is_empty(s: str) -> bool:
    """Return True if s has no code points."""
    return len(s) == 0


def is_blank(s: str) -> bool:
    """Return True if each code point of s is ASCII white space."""
    # str.isspace is False for the empty string, and it also accepts
    # Unicode spaces, so the code strips the six code points instead.
    return s.strip(ASCII_SPACE) == ""
