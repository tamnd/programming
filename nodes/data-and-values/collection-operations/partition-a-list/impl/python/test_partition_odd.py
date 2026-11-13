# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/partition-a-list/.

import pytest

from partition_odd import partition_odd

CASES = {
    "mixed": ([1, 2, 3, 4, 5], ([1, 3, 5], [2, 4])),
    "tens": ([10, 15, 20, 25], ([15, 25], [10, 20])),
    "empty": ([], ([], [])),
    "all-odd": ([1, 3], ([1, 3], [])),
    "all-even": ([2, 4], ([], [2, 4])),
    "negative": ([-3, -2, -1, 0], ([-3, -1], [-2, 0])),
    "zero": ([0], ([], [0])),
    "order": ([9, 8, 1, 2], ([9, 1], [8, 2])),
    "extremes": ([-2147483648, 2147483647], ([2147483647], [-2147483648])),
    "duplicates": ([3, 3, 2, 2], ([3, 3], [2, 2])),
}


@pytest.mark.parametrize("name", CASES)
def test_partition_odd(name):
    items, want = CASES[name]
    assert partition_odd(items) == want
