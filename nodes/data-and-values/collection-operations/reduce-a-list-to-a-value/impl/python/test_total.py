# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

import pytest

from total import total

CASES = {
    "small": ([1, 2, 3, 4], 10),
    "mixed": ([10, -3, 5], 12),
    "empty": ([], 0),
    "single": ([7], 7),
    "negative": ([-1, -2, -3], -6),
    "zero-sum": ([5, -5], 0),
    "max": ([2147483647], 2147483647),
    "min": ([-2147483648], -2147483648),
    "to-the-limit": ([2147483646, 1], 2147483647),
    "partial-overflow": ([2147483647, 1, -1], 2147483647),
    "partial-underflow": ([-2147483648, -1, 1], -2147483648),
}


@pytest.mark.parametrize("name", CASES)
def test_total(name):
    items, want = CASES[name]
    assert total(items) == want
