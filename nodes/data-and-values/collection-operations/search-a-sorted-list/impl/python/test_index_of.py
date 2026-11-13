# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-a-sorted-list/.

import pytest

from index_of import index_of

CASES = {
    "middle": ([1, 3, 5, 7, 9], 5, 2),
    "first": ([1, 3, 5, 7, 9], 1, 0),
    "last": ([1, 3, 5, 7, 9], 9, 4),
    "missing": ([1, 3, 5, 7, 9], 4, -1),
    "below": ([1, 3, 5], 0, -1),
    "above": ([1, 3, 5], 6, -1),
    "empty": ([], 1, -1),
    "single-hit": ([7], 7, 0),
    "single-miss": ([7], 8, -1),
    "duplicates": ([1, 2, 2, 2, 3], 2, 1),
    "all-same": ([4, 4, 4, 4], 4, 0),
    "negative": ([-9, -5, -1, 0], -5, 1),
    "extremes": ([-2147483648, 0, 2147483647], 2147483647, 2),
    "min-target": ([-2147483648, 0, 2147483647], -2147483648, 0),
}


@pytest.mark.parametrize("name", CASES)
def test_index_of(name):
    items, target, want = CASES[name]
    assert index_of(items, target) == want
