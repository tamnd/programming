# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/destructure-a-value/.

import pytest

from manhattan import manhattan

CASES = {
    "first": ([1, 2], [4, 6], 7),
    "on-a-line": ([0, 0], [10, 0], 10),
    "down-left": ([5, 5], [2, 1], 7),
    "mixed-signs": ([3, -1], [-2, 4], 10),
    "same-point": ([7, 7], [7, 7], 0),
    "negative": ([-3, -4], [-1, -1], 5),
    "large": ([1000000, -1000000], [-1000000, 1000000], 4000000),
}


@pytest.mark.parametrize("name", CASES)
def test_manhattan(name):
    p, q, want = CASES[name]
    assert manhattan(p, q) == want
