# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-two-lists/.

import pytest

from concat import concat

CASES = {
    "two-pairs": ([1, 2], [3, 4], [1, 2, 3, 4]),
    "uneven": ([5], [6, 7, 8], [5, 6, 7, 8]),
    "both-empty": ([], [], []),
    "left-empty": ([], [1, 2], [1, 2]),
    "right-empty": ([1, 2], [], [1, 2]),
    "duplicates": ([1, 1], [1], [1, 1, 1]),
    "order": ([3, 1], [2], [3, 1, 2]),
    "limit": ([2147483647], [-2147483648], [2147483647, -2147483648]),
}


@pytest.mark.parametrize("name", CASES)
def test_concat(name):
    a, b, want = CASES[name]
    a0, b0 = list(a), list(b)
    assert concat(a, b) == want
    assert (a, b) == (a0, b0)
