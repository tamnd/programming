# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

import pytest

from roman import from_roman, to_roman

CASES = [
    ("year-1990", 1990, "MCMXC"),
    ("year-2008", 2008, "MMVIII"),
    ("each-symbol-once", 1666, "MDCLXVI"),
    ("one", 1, "I"),
    ("four", 4, "IV"),
    ("nine", 9, "IX"),
    ("forty-nine", 49, "XLIX"),
    ("ninety-nine", 99, "XCIX"),
    ("four-hundred-forty-four", 444, "CDXLIV"),
    ("year-1994", 1994, "MCMXCIV"),
    ("longest", 3888, "MMMDCCCLXXXVIII"),
    ("largest", 3999, "MMMCMXCIX"),
]


@pytest.mark.parametrize(("name", "n", "want"), CASES, ids=[c[0] for c in CASES])
def test_to_roman(name, n, want):
    assert to_roman(n) == want
    assert from_roman(want) == n


def test_round_trip():
    for n in range(1, 4000):
        assert from_roman(to_roman(n)) == n


@pytest.mark.parametrize("s", ["", "IIII", "IC", "VX", "MMMM", "iv", "ABC", "IIV", "XM"])
def test_invalid(s):
    with pytest.raises(ValueError):
        from_roman(s)


@pytest.mark.parametrize("n", [0, -1, 4000])
def test_out_of_range(n):
    with pytest.raises(ValueError):
        to_roman(n)
