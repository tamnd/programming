# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-an-index/.

import pytest

from positions import positions

CASES = {
    "two-matches": ((["a", "b", "a", "c"], "a"), [0, 2]),
    "words": ((["red", "green", "blue", "green"], "green"), [1, 3]),
    "empty": (([], "a"), []),
    "none": ((["a", "b"], "z"), []),
    "all": ((["x", "x", "x"], "x"), [0, 1, 2]),
    "last": ((["a", "b", "c"], "c"), [2]),
    "case": ((["A", "a"], "a"), [1]),
    "empty-string": ((["", "a", ""], ""), [0, 2]),
}


@pytest.mark.parametrize("name", CASES)
def test_positions(name):
    args, want = CASES[name]
    assert positions(*args) == want
