# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-generic-function/.

import pytest

from count_greater import count_greater

CASES = {
    "some": ([34, 50, 25, 100, 65], 50, 2),
    "all": ([3, 4, 5], 2, 3),
    "none": ([3, 4, 5], 9, 0),
    "empty": ([], 0, 0),
    "equal-not-counted": ([5, 5, 5], 5, 0),
    "duplicates": ([7, 1, 7, 7], 6, 3),
    "negative": ([-3, -1, 0, 2], -2, 3),
    "min-limit": ([-2147483648, 0, 2147483647], -2147483648, 2),
    "max-limit": ([2147483647, 1], 2147483647, 0),
    "single": ([1], 0, 1),
}


@pytest.mark.parametrize("name", CASES)
def test_count_greater(name):
    items, limit, want = CASES[name]
    assert count_greater(items, limit) == want


def test_strings():
    assert count_greater(["pear", "apple", "plum"], "orange") == 2


def test_floats():
    assert count_greater([0.5, 1.5, 2.5], 1.0) == 2
