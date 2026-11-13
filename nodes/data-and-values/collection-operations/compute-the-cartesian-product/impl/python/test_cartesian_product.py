# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-cartesian-product/.

import pytest

from cartesian_product import cartesian_product

CASES = {
    "two": ([[1, 2], [3, 4]], [[1, 3], [1, 4], [2, 3], [2, 4]]),
    "three": ([[1, 2], [3], [4, 5]], [[1, 3, 4], [1, 3, 5], [2, 3, 4], [2, 3, 5]]),
    "no-lists": ([], [[]]),
    "one-empty": ([[1, 2], []], []),
    "single": ([[7, 8, 9]], [[7], [8], [9]]),
    "duplicates": ([[1, 1], [2]], [[1, 2], [1, 2]]),
    "negative": ([[-1], [-2, -3]], [[-1, -2], [-1, -3]]),
    "extremes": ([[-2147483648, 2147483647], [0]], [[-2147483648, 0], [2147483647, 0]]),
}


@pytest.mark.parametrize("name", CASES)
def test_cartesian_product(name):
    lists, want = CASES[name]
    assert cartesian_product(lists) == want
