# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-an-array/.

import pytest

from filled import filled

CASES = {
    "three-sevens": (3, 7, [7, 7, 7]),
    "five-zeros": (5, 0, [0, 0, 0, 0, 0]),
    "empty": (0, 7, []),
    "single": (1, 42, [42]),
    "zero": (2, 0, [0, 0]),
    "negative": (2, -1, [-1, -1]),
    "max-int": (2, 2147483647, [2147483647, 2147483647]),
    "min-int": (2, -2147483648, [-2147483648, -2147483648]),
    "ten": (10, 1, [1, 1, 1, 1, 1, 1, 1, 1, 1, 1]),
}


@pytest.mark.parametrize("name", CASES)
def test_filled(name):
    n, value, want = CASES[name]
    assert filled(n, value) == want
