# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/build-a-string-efficiently/.

import pytest

from number_line import number_line

CASES = {
    "zero": (0, ""),
    "one": (1, "1"),
    "three": (3, "1,2,3"),
    "five": (5, "1,2,3,4,5"),
    "seven": (7, "1,2,3,4,5,6,7"),
    "nine": (9, "1,2,3,4,5,6,7,8,9"),
    "ten": (10, "1,2,3,4,5,6,7,8,9,10"),
}

# Long results: the length, the start and the end.
LONG = {
    "hundred": (100, 291, ",99,100"),
    "hundred-and-one": (101, 295, ",100,101"),
    "thousand": (1000, 3892, ",999,1000"),
    "large": (100000, 588894, ",99999,100000"),
}


@pytest.mark.parametrize("name", CASES)
def test_exact(name):
    n, want = CASES[name]
    assert number_line(n) == want


@pytest.mark.parametrize("name", LONG)
def test_long(name):
    n, size, end = LONG[name]
    got = number_line(n)
    assert len(got) == size
    assert got.startswith("1,2,3,")
    assert got.endswith(end)
