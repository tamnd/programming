# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

import pytest

from count_items import count_items

CASES = {
    "mixed": ([3, 1, 3, 2, 1, 3], [(1, 2), (2, 1), (3, 3)]),
    "same": ([5, 5, 5], [(5, 3)]),
    "empty": ([], []),
    "single": ([7], [(7, 1)]),
    "negative": ([-1, 2, -1, 0], [(-1, 2), (0, 1), (2, 1)]),
    "distinct": ([4, 2, 9], [(2, 1), (4, 1), (9, 1)]),
    "order": ([9, 8, 9], [(8, 1), (9, 2)]),
    "zero": ([0, 0], [(0, 2)]),
    "extremes": ([2147483647, -2147483648, 2147483647], [(-2147483648, 1), (2147483647, 2)]),
}


@pytest.mark.parametrize("name", CASES)
def test_count_items(name):
    items, want = CASES[name]
    assert count_items(items) == want
