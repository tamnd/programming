# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/flatten-a-nested-list/.

import pytest

from flatten import flatten

CASES = {
    "three": ([[1, 2], [3], [4, 5, 6]], [1, 2, 3, 4, 5, 6]),
    "two": ([[7], [8, 9]], [7, 8, 9]),
    "empty": ([], []),
    "empty-inner": ([[], [1], []], [1]),
    "all-empty": ([[], []], []),
    "single": ([[5]], [5]),
    "one-list": ([[1, 2, 3]], [1, 2, 3]),
    "negative": ([[-1, -2], [3]], [-1, -2, 3]),
    "extremes": ([[-2147483648], [2147483647, 0]], [-2147483648, 2147483647, 0]),
}


@pytest.mark.parametrize("name", CASES)
def test_flatten(name):
    lists, want = CASES[name]
    assert flatten(lists) == want
