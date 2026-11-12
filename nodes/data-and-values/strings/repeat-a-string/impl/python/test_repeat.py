# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/repeat-a-string/.

import pytest

from repeat import repeat

# The case many has the 200 chars of 100 copies of ab in one literal.
CASES = {
    "word": ("ab", 3, "ababab"),
    "one": ("abc", 1, "abc"),
    "dash": ("-", 5, "-----"),
    "zero": ("abc", 0, ""),
    "empty": ("", 5, ""),
    "empty-zero": ("", 0, ""),
    "many": ("ab", 100, "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab"),
    "astral": ("\U0001f642", 3, "\U0001f642\U0001f642\U0001f642"),
    "combining": ("e\u0301", 2, "e\u0301e\u0301"),
}


@pytest.mark.parametrize("name", CASES)
def test_repeat(name):
    s, n, want = CASES[name]
    assert repeat(s, n) == want
