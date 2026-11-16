# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-division-by-zero/.
import pytest

from safe_div import INT64_MAX, INT64_MIN, safe_div

CASES = {
    "exact": (12, 4, 3),
    "remainder-dropped": (7, 2, 3),
    "zero-dividend": (0, 5, 0),
    "zero-divisor": (5, 0, None),
    "negative-by-zero": (-5, 0, None),
    "zero-by-zero": (0, 0, None),
    "negative-dividend": (-7, 2, -3),
    "negative-divisor": (7, -2, -3),
    "both-negative": (-7, -2, 3),
    "min-by-minus-one": (INT64_MIN, -1, None),
    "min-by-one": (INT64_MIN, 1, INT64_MIN),
    "max-by-minus-one": (INT64_MAX, -1, -INT64_MAX),
    "min-by-max": (INT64_MIN, INT64_MAX, -1),
}


@pytest.mark.parametrize("case", CASES)
def test_safe_div(case):
    a, b, want = CASES[case]
    assert safe_div(a, b) == want


def test_floor_division_differs():
    assert -7 // 2 == -4
    with pytest.raises(ZeroDivisionError):
        5 // 0
