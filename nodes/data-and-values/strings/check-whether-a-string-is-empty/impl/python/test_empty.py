# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

import pytest

from empty import is_blank, is_empty

# U+00A0 is NO-BREAK SPACE, U+200B is ZERO WIDTH SPACE, and U+3000 is IDEOGRAPHIC SPACE. None of them is ASCII white space.
EMPTY = {
    "letter": ("a", False),
    "word": ("hello", False),
    "empty": ("", True),
    "space": (" ", False),
    "newline": ("\n", False),
    "nbsp": ("\u00a0", False),
    "zero-width": ("\u200b", False),
    "combining": ("\u0301", False),
    "astral": ("\U0001f642", False),
    "cjk": ("日", False),
}
BLANK = {
    "blank-empty": ("", True),
    "blank-space": (" ", True),
    "blank-mixed": (" \t\n\r", True),
    "blank-vt-ff": ("\v\f", True),
    "blank-letter": ("a", False),
    "blank-padded": ("  a  ", False),
    "blank-nbsp": ("\u00a0", False),
    "blank-zero-width": ("\u200b", False),
    "blank-ideographic": ("\u3000", False),
    "blank-astral": (" \U0001f642 ", False),
}


@pytest.mark.parametrize("name", EMPTY)
def test_is_empty(name):
    s, want = EMPTY[name]
    assert is_empty(s) is want


@pytest.mark.parametrize("name", BLANK)
def test_is_blank(name):
    s, want = BLANK[name]
    assert is_blank(s) is want
