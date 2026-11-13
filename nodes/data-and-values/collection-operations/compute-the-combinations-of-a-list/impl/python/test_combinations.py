# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

import pytest

from combinations import combinations

CASES = {
    "two-of-four": ([1, 2, 3, 4], 2, [[1, 2], [1, 3], [1, 4], [2, 3], [2, 4], [3, 4]]),
    "three-of-four": ([1, 2, 3, 4], 3, [[1, 2, 3], [1, 2, 4], [1, 3, 4], [2, 3, 4]]),
    "zero-k": ([1, 2], 0, [[]]),
    "empty": ([], 0, [[]]),
    "k-too-large": ([1, 2], 3, []),
    "all": ([1, 2, 3], 3, [[1, 2, 3]]),
    "duplicates": ([1, 1, 2], 2, [[1, 1], [1, 2], [1, 2]]),
    "unsorted": ([3, 1, 2], 2, [[3, 1], [3, 2], [1, 2]]),
    "extremes": ([-2147483648, 2147483647], 1, [[-2147483648], [2147483647]]),
}


@pytest.mark.parametrize("name", CASES)
def test_combinations(name):
    items, k, want = CASES[name]
    assert combinations(items, k) == want
