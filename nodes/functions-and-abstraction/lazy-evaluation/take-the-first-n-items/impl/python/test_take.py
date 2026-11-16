# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/take-the-first-n-items/.

import pytest

from take import take, take_naturals

CASES = {
    "first-three": ([1, 2, 3, 4, 5], 3, [1, 2, 3]),
    "first-two": ([10, 20, 30, 40], 2, [10, 20]),
    "zero": ([1, 2], 0, []),
    "empty": ([], 3, []),
    "more-than-length": ([1, 2], 5, [1, 2]),
    "equal-length": ([4, 5, 6], 3, [4, 5, 6]),
    "one": ([9, 8], 1, [9]),
    "negative": ([-1, -2, -3], 2, [-1, -2]),
    "extremes": ([-2147483648, 2147483647, 0], 2, [-2147483648, 2147483647]),
}

INFINITE = {
    "from-zero": (0, 5, [0, 1, 2, 3, 4]),
    "from-ten": (10, 3, [10, 11, 12]),
    "none": (7, 0, []),
    "one": (0, 1, [0]),
    "large-start": (999998, 2, [999998, 999999]),
}


@pytest.mark.parametrize("name", CASES)
def test_take(name):
    items, n, want = CASES[name]
    assert take(items, n) == want


@pytest.mark.parametrize("name", INFINITE)
def test_take_naturals(name):
    start, n, want = INFINITE[name]
    assert take_naturals(start, n) == want


def test_take_returns_a_copy():
    items = [1, 2, 3]
    first = take(items, 2)
    first.append(99)
    assert items == [1, 2, 3]
