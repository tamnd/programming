# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-function/.

import pytest

from clamp import clamp

CASES = {
    "inside": (5, 1, 10, 5),
    "below": (-3, 0, 10, 0),
    "above": (42, 0, 10, 10),
    "at-low": (0, 0, 10, 0),
    "at-high": (10, 0, 10, 10),
    "equal-bounds": (7, 3, 3, 3),
    "negative-range": (-50, -20, -10, -20),
    "zero": (0, -5, 5, 0),
    "int-max": (2147483647, -100, 100, 100),
    "int-min": (-2147483648, -100, 100, -100),
    "full-range": (-2147483648, -2147483648, 2147483647, -2147483648),
}


@pytest.mark.parametrize("name", CASES)
def test_clamp(name):
    value, low, high, want = CASES[name]
    assert clamp(value, low, high) == want


def test_keyword_arguments():
    assert clamp(value=15, low=0, high=10) == 10
