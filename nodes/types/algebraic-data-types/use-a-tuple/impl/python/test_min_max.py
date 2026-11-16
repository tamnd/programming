# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-tuple/.

import pytest

from min_max import min_max

CASES = {
    "mixed": ([3, 1, 2], (1, 3)),
    "longer": ([4, 9, -2, 7, 0], (-2, 9)),
    "one": ([5], (5, 5)),
    "negative": ([-3, -1, -7], (-7, -1)),
    "all-same": ([2, 2, 2], (2, 2)),
    "int32-extremes": ([2147483647, 0, -2147483648], (-2147483648, 2147483647)),
    "sorted": ([1, 2, 3, 4], (1, 4)),
    "reversed": ([4, 3, 2, 1], (1, 4)),
}


@pytest.mark.parametrize("case", CASES)
def test_min_max(case):
    numbers, want = CASES[case]
    before = list(numbers)
    lo, hi = min_max(numbers)  # sequence unpacking
    assert (lo, hi) == want
    assert numbers == before


def test_result_is_a_tuple():
    got = min_max([3, 1, 2])
    assert isinstance(got, tuple)
    assert got[0] == 1 and got[1] == 3
