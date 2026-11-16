# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-integer-arithmetic/.
import pytest

from arith import arith

CASES = {
    "small": (7, 2, 3, [9, 5, 14, 3, 1, 343]),
    "exact-division": (20, 5, 2, [25, 15, 100, 4, 0, 400]),
    "negative-dividend": (-7, 2, 3, [-5, -9, -14, -3, -1, -343]),
    "negative-divisor": (7, -2, 2, [5, 9, -14, -3, 1, 49]),
    "both-negative": (-7, -2, 1, [-9, -5, 14, 3, -1, -7]),
    "zero-dividend": (0, 5, 0, [5, -5, 0, 0, 0, 1]),
    "zero-power": (5, 3, 0, [8, 2, 15, 1, 2, 1]),
    "divisor-larger": (3, 7, 1, [10, -4, 21, 0, 3, 3]),
    "largest": (100000, 100000, 3, [200000, 0, 10000000000, 1, 0, 10**15]),
    "smallest": (-100000, 1, 3, [-99999, -100001, -100000, -100000, 0, -(10**15)]),
}


@pytest.mark.parametrize("case", CASES)
def test_arith(case):
    a, b, n, want = CASES[case]
    assert arith(a, b, n) == want


def test_python_operators_floor():
    assert (-7 // 2, -7 % 2) == (-4, 1)
    assert divmod(-7, 2) == (-4, 1)
