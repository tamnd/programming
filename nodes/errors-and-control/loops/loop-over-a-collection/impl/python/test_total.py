# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-a-collection/.

import pytest

from total import total

CASES = {
    "three": ([1, 2, 3], 6),
    "mixed": ([10, -4, 7, 0, 3], 16),
    "empty": ([], 0),
    "single": ([7], 7),
    "negative": ([-5, 2, -1], -4),
    "limit": ([2147483647, 2147483647, 2], 4294967296),
    "limit-low": ([-2147483648, -2147483648], -4294967296),
}


@pytest.mark.parametrize("name", CASES)
def test_total(name):
    items, want = CASES[name]
    assert total(items) == want
