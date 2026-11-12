# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-strings/.

import pytest

from compare import compare, equals

# U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
COMPARE = {
    "less": ("apple", "banana", -1),
    "equal": ("apple", "apple", 0),
    "greater": ("pear", "apple", 1),
    "both-empty": ("", "", 0),
    "empty-first": ("", "a", -1),
    "prefix": ("app", "apple", -1),
    "prefix-longer": ("apple", "app", 1),
    "upper-before-lower": ("Zebra", "apple", -1),
    "lower-after-upper": ("a", "A", 1),
    "digits": ("10", "9", -1),
    "non-ascii": ("\u00e9", "z", 1),
    "astral-after-bmp": ("\uff61", "\U0001f642", -1),
    "astral": ("\U0001f642", "\U0001f643", -1),
    "combining": ("cafe\u0301", "caf\u00e9", -1),
}
EQUAL = {
    "eq-same": ("abc", "abc", True),
    "eq-case": ("abc", "ABC", False),
    "eq-empty": ("", "", True),
    "eq-combining": ("cafe\u0301", "caf\u00e9", False),
    "eq-astral": ("\U0001f642", "\U0001f642", True),
    "eq-trailing-space": ("abc", "abc ", False),
}


@pytest.mark.parametrize("name", COMPARE)
def test_compare(name):
    a, b, want = COMPARE[name]
    assert compare(a, b) == want


@pytest.mark.parametrize("name", EQUAL)
def test_equals(name):
    a, b, want = EQUAL[name]
    assert equals(a, b) is want
