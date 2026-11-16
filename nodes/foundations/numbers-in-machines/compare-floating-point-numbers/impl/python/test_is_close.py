# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compare-floating-point-numbers/.
import math

import pytest

from is_close import is_close

CASES = {
    "equal": (1.5, 1.5, 1e-9, 0.0, True),
    "sum-of-tenths": (0.30000000000000004, 0.3, 1e-9, 0.0, True),
    "different": (1.0, 1.1, 1e-9, 0.0, False),
    "large-values": (10000000000.0, 10000000001.0, 1e-9, 0.0, True),
    "zero-and-zero": (0.0, 0.0, 0.0, 0.0, True),
    "one-ulp-apart": (1.0, 1.0000000000000002, 0.0, 0.0, False),
    "near-zero-relative": (1e-12, 0.0, 1e-9, 0.0, False),
    "near-zero-absolute": (1e-12, 0.0, 1e-9, 1e-9, True),
    "opposite-signs": (1e-10, -1e-10, 1e-9, 0.0, False),
    "opposite-signs-absolute": (1e-10, -1e-10, 1e-9, 1e-9, True),
    "on-the-boundary": (10.0, 9.0, 0.1, 0.0, True),
    "swapped-boundary": (9.0, 10.0, 0.1, 0.0, True),
    "past-the-boundary": (10.0, 8.875, 0.1, 0.0, False),
    "negative-values": (-1.0, -1.0000000001, 1e-9, 0.0, True),
}


@pytest.mark.parametrize("case", CASES)
def test_is_close(case):
    a, b, rel_tol, abs_tol, want = CASES[case]
    assert is_close(a, b, rel_tol, abs_tol) is want
    assert is_close(b, a, rel_tol, abs_tol) is want


def test_non_finite():
    nan, inf = math.nan, math.inf
    assert not is_close(nan, nan, 1e-9, 0.0)
    assert not is_close(nan, 1.0, 1e-9, 1.0)
    assert is_close(inf, inf, 1e-9, 0.0)
    assert not is_close(inf, -inf, 1e-9, 0.0)
    assert not is_close(inf, 1e308, 1e-9, 0.0)


def test_equality_and_ulp():
    assert 0.1 + 0.2 != 0.3
    assert math.nextafter(1.0, 2.0) == 1.0000000000000002
    assert math.ulp(1.0) == 2.0**-52
