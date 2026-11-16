# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-integer-overflow/.
import pytest

from checked_add import INT64_MAX, INT64_MIN, checked_add

CASES = {
    "small": (2, 3, 5),
    "negative": (-7, 4, -3),
    "beyond-32-bit": (4000000000, 5000000000, 9000000000),
    "max-plus-zero": (INT64_MAX, 0, INT64_MAX),
    "max-plus-one": (INT64_MAX, 1, None),
    "max-plus-max": (INT64_MAX, INT64_MAX, None),
    "min-plus-zero": (INT64_MIN, 0, INT64_MIN),
    "min-minus-one": (INT64_MIN, -1, None),
    "min-plus-min": (INT64_MIN, INT64_MIN, None),
    "min-plus-max": (INT64_MIN, INT64_MAX, -1),
    "max-minus-one": (INT64_MAX, -1, INT64_MAX - 1),
}


@pytest.mark.parametrize("case", CASES)
def test_checked_add(case):
    a, b, want = CASES[case]
    assert checked_add(a, b) == want


def test_limits():
    assert INT64_MAX == 9223372036854775807
    assert INT64_MIN == -9223372036854775808
