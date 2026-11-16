# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pad-a-string/.

import pytest

from pad import pad

CASES = [
    ("zeros", "7", 3, "0", ("007", "700")),
    ("spaces", "abc", 6, " ", ("   abc", "abc   ")),
    ("stars", "abc", 6, "*", ("***abc", "abc***")),
    ("empty-string", "", 3, "-", ("---", "---")),
    ("width-zero", "abc", 0, " ", ("abc", "abc")),
    ("exact-width", "abcd", 4, ".", ("abcd", "abcd")),
    ("shorter-width", "hello", 3, " ", ("hello", "hello")),
    ("negative-number-text", "-42", 6, "0", ("000-42", "-42000")),
    ("fill-same-as-text", "aa", 5, "a", ("aaaaa", "aaaaa")),
    ("one-char-wide", "x", 1, "_", ("x", "x")),
    ("spaces-inside", "a b", 7, ".", ("....a b", "a b....")),
]


@pytest.mark.parametrize(("name", "s", "width", "fill", "want"), CASES, ids=[c[0] for c in CASES])
def test_pad(name, s, width, fill, want):
    assert pad(s, width, fill) == want
