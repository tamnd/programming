# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-a-list/.

import pytest

from keep_odd import keep_odd

CASES = {
    "mixed": ([1, 2, 3, 4, 5], [1, 3, 5]),
    "tens": ([10, 15, 20, 25], [15, 25]),
    "empty": ([], []),
    "none-odd": ([2, 4, 6], []),
    "all-odd": ([1, 3, 5], [1, 3, 5]),
    "negative": ([-3, -2, -1, 0], [-3, -1]),
    "zero": ([0], []),
    "order": ([9, 1, 7], [9, 1, 7]),
    "extremes": ([-2147483648, 2147483647], [2147483647]),
    "duplicates": ([3, 3, 2, 3], [3, 3, 3]),
}


@pytest.mark.parametrize("name", CASES)
def test_keep_odd(name):
    items, want = CASES[name]
    assert keep_odd(items) == want
