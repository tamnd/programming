# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-queue/.

import pytest

from round_robin import finish_order

CASES = {
    "example": ([3, 1, 2], 1, [1, 2, 0]),
    "slice-two": ([4, 1, 3, 2], 2, [1, 3, 0, 2]),
    "empty": ([], 3, []),
    "one": ([7], 2, [0]),
    "big-slice": ([3, 1, 2], 10, [0, 1, 2]),
    "slice-one": ([2, 2, 1], 1, [2, 0, 1]),
    "equal-work": ([2, 2, 2], 1, [0, 1, 2]),
    "long-job-first": ([10, 1, 1], 3, [1, 2, 0]),
}


@pytest.mark.parametrize("case", CASES)
def test_finish_order(case):
    work, slice_, want = CASES[case]
    before = list(work)
    assert finish_order(work, slice_) == want
    assert work == before
