# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pattern-match-on-a-value/.

import pytest

from classify import classify

CASES = {
    "x-axis": (5, 0, "x-axis"),
    "y-axis": (0, -3, "y-axis"),
    "diagonal": (4, 4, "diagonal"),
    "other": (2, 7, "other"),
    "origin": (0, 0, "origin"),
    "negative-diagonal": (-2, -2, "diagonal"),
    "anti-diagonal": (3, -3, "other"),
    "negative-x-axis": (-7, 0, "x-axis"),
    "one-one": (1, 1, "diagonal"),
}


@pytest.mark.parametrize("name", CASES)
def test_classify(name):
    x, y, want = CASES[name]
    assert classify(x, y) == want
