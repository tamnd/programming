# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/zip-two-lists/.

import pytest

from zip_pairs import zip_pairs

CASES = {
    "same-length": ([1, 2, 3], [4, 5, 6], [(1, 4), (2, 5), (3, 6)]),
    "tens": ([10, 20], [30, 40], [(10, 30), (20, 40)]),
    "empty": ([], [], []),
    "first-empty": ([], [1, 2], []),
    "second-empty": ([1, 2], [], []),
    "first-shorter": ([1], [7, 8], [(1, 7)]),
    "second-shorter": ([1, 2, 3], [9], [(1, 9)]),
    "negative": ([-1, -2], [3, -4], [(-1, 3), (-2, -4)]),
    "extremes": ([-2147483648, 2147483647], [2147483647, -2147483648], [(-2147483648, 2147483647), (2147483647, -2147483648)]),
    "duplicates": ([5, 5], [5, 5], [(5, 5), (5, 5)]),
}


@pytest.mark.parametrize("name", CASES)
def test_zip_pairs(name):
    a, b, want = CASES[name]
    assert zip_pairs(a, b) == want
