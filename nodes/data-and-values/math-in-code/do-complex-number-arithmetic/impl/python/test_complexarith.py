# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

import math

import pytest

from complexarith import complex_arithmetic

CASES = [
    ("textbook", [3.0, 4.0], [1.0, -2.0], [4.0, 2.0, 11.0, -2.0, -1.0, 2.0, 5.0, 0.9272952180016122]),
    ("rosetta", [1.0, 1.0], [3.14159, 1.25], [4.14159, 2.25, 1.8915899999999999, 4.39159, 0.38414593243590134, 0.16546321590504276, 1.4142135623730951, 0.7853981633974483]),
    ("real-only", [6.0, 0.0], [3.0, 0.0], [9.0, 0.0, 18.0, 0.0, 2.0, 0.0, 6.0, 0.0]),
    ("imaginary-unit", [0.0, 1.0], [0.0, 1.0], [0.0, 2.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.5707963267948966]),
    ("negative-real", [-2.0, 0.0], [1.0, 0.0], [-1.0, 0.0, -2.0, 0.0, -2.0, 0.0, 2.0, 3.141592653589793]),
    ("zero", [0.0, 0.0], [2.0, -3.0], [2.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]),
    ("negative-phase", [1.0, -1.0], [-1.0, -1.0], [0.0, -2.0, -2.0, 0.0, 0.0, 1.0, 1.4142135623730951, -0.7853981633974483]),
    ("imaginary-divisor", [2.0, 4.0], [0.0, 2.0], [2.0, 6.0, -8.0, 4.0, 2.0, -1.0, 4.47213595499958, 1.1071487177940904]),
    ("large-divisor", [1e+150, 1e+150], [1e+155, 1e+155], [1.00001e+155, 1.00001e+155, 0.0, 2e+305, 9.999999999999999e-06, 0.0, 1.414213562373095e+150, 0.7853981633974483]),
]


def close(got, want):
    return math.isclose(got, want, rel_tol=1e-12, abs_tol=1e-9)


@pytest.mark.parametrize(("name", "a", "b", "want"), CASES, ids=[c[0] for c in CASES])
def test_complex_arithmetic(name, a, b, want):
    got = complex_arithmetic(a, b)
    assert len(got) == 8
    assert all(close(g, w) for g, w in zip(got, want)), got


def test_divide_by_zero_raises():
    with pytest.raises(ZeroDivisionError):
        complex_arithmetic([1, 1], [0, 0])
