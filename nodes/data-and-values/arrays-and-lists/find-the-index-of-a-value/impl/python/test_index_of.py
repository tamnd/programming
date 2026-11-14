# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-index-of-a-value/.

import pytest

from index_of import index_of

CASES = {
    "middle": ([4, 7, 1], 7, 1),
    "absent": ([4, 7, 1], 9, -1),
    "empty": ([], 0, -1),
    "single": ([5], 5, 0),
    "single-absent": ([5], 0, -1),
    "first": ([7, 8, 9], 7, 0),
    "last": ([7, 8, 9], 9, 2),
    "duplicates": ([3, 5, 3, 5], 5, 1),
    "all-same": ([2, 2, 2], 2, 0),
    "negative": ([0, -1, -2], -2, 2),
    "limit": ([2147483647, -2147483648], -2147483648, 1),
    "limit-absent": ([2147483647], -2147483648, -1),
}


@pytest.mark.parametrize("name", CASES)
def test_index_of(name):
    items, value, want = CASES[name]
    items0 = list(items)
    assert index_of(items, value) == want
    assert items == items0
