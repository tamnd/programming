# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/trim-whitespace/.

"""Trim the ASCII white space from a string."""

# The 6 ASCII white space characters. str.strip() with no argument also
# removes U+00A0, U+3000 and the other white space of Unicode.
WHITESPACE = " \t\n\v\f\r"


def trim(s: str) -> str:
    """Return s with the white space at the start and at the end removed."""
    return s.strip(WHITESPACE)


def trim_start(s: str) -> str:
    """Return s with the white space at the start removed."""
    return s.lstrip(WHITESPACE)
