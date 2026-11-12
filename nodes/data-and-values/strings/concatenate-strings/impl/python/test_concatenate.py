# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-strings/.

import pytest

from concatenate import concat, concat_number

CASES = {
    "hello-world": ("Hello, ", "world", "Hello, world"),
    "two-words": ("good", "bye", "goodbye"),
    "keep-spaces": ("a ", " b", "a  b"),
    "empty-both": ("", "", ""),
    "empty-left": ("", "abc", "abc"),
    "empty-right": ("abc", "", "abc"),
    "non-ascii": ("naï", "ve", "naïve"),
    "astral": ("a", "🙂", "a🙂"),
    "combining-mark": ("e", "\u0301", "e\u0301"),
    "flag-halves": ("\U0001f1eb", "\U0001f1f7", "🇫🇷"),
    "digit-strings": ("1", "2", "12"),
}

NUMBER_CASES = {
    "positive": ("item", 3, "item3"),
    "zero": ("n=", 0, "n=0"),
    "negative": ("x", -5, "x-5"),
    "empty-prefix": ("", 42, "42"),
    "int32-max": ("max=", 2147483647, "max=2147483647"),
    "int32-min": ("min=", -2147483648, "min=-2147483648"),
}


@pytest.mark.parametrize("name", CASES)
def test_concat(name):
    a, b, want = CASES[name]
    assert concat(a, b) == want


@pytest.mark.parametrize("name", NUMBER_CASES)
def test_concat_number(name):
    s, n, want = NUMBER_CASES[name]
    assert concat_number(s, n) == want
