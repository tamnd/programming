# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-priority-queue/.

import pytest

from serve_order import serve_order

LO, HI = -2147483648, 2147483647

CASES = {
    "example": ([2, 5, 1, 5], [1, 3, 0, 2]),
    "longer": ([3, 1, 4, 1, 5, 9, 2, 6], [5, 7, 4, 2, 0, 6, 1, 3]),
    "empty": ([], []),
    "one": ([7], [0]),
    "ties": ([4, 4, 4, 4], [0, 1, 2, 3]),
    "negative": ([-3, -1, -2], [1, 2, 0]),
    "int32-extremes": ([LO, HI, 0, LO], [1, 2, 0, 3]),
    "sorted": ([1, 2, 3], [2, 1, 0]),
    "reversed": ([3, 2, 1], [0, 1, 2]),
}


@pytest.mark.parametrize("case", CASES)
def test_serve_order(case):
    priorities, want = CASES[case]
    before = list(priorities)
    assert serve_order(priorities) == want
    assert priorities == before
