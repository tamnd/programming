# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

import pytest

from contains import contains

CASES = {
    "present": ([1, 2, 3], 2, True),
    "absent": ([1, 2, 3], 4, False),
    "empty": ([], 0, False),
    "single-yes": ([5], 5, True),
    "single-no": ([5], 0, False),
    "first": ([7, 8, 9], 7, True),
    "last": ([7, 8, 9], 9, True),
    "duplicates": ([2, 2, 2], 2, True),
    "negative": ([-1, -2, -3], -2, True),
    "zero-absent": ([-1, 1], 0, False),
    "limit": ([2147483647, -2147483648], -2147483648, True),
    "limit-absent": ([2147483647], -2147483648, False),
}


@pytest.mark.parametrize("name", CASES)
def test_contains(name):
    items, value, want = CASES[name]
    items0 = list(items)
    assert contains(items, value) is want
    assert items == items0
