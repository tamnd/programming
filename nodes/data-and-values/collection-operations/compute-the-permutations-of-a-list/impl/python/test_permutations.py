# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

import pytest

from permutations import permutations

CASES = {
    "three": ([1, 2, 3], [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]),
    "two": ([5, 6], [[5, 6], [6, 5]]),
    "empty": ([], [[]]),
    "single": ([7], [[7]]),
    "duplicates": ([1, 1, 2], [[1, 1, 2], [1, 2, 1], [1, 1, 2], [1, 2, 1], [2, 1, 1], [2, 1, 1]]),
    "unsorted": ([3, 1, 2], [[3, 1, 2], [3, 2, 1], [1, 3, 2], [1, 2, 3], [2, 3, 1], [2, 1, 3]]),
    "extremes": ([-2147483648, 2147483647], [[-2147483648, 2147483647], [2147483647, -2147483648]]),
}


@pytest.mark.parametrize("name", CASES)
def test_permutations(name):
    items, want = CASES[name]
    assert permutations(items) == want
