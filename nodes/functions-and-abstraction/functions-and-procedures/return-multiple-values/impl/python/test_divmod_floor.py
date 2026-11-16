# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/return-multiple-values/.

import pytest

from divmod_floor import div_mod

CASES = {
    "positive": (17, 5, 3, 2),
    "exact": (20, 4, 5, 0),
    "smaller-dividend": (3, 10, 0, 3),
    "negative-dividend": (-17, 5, -4, 3),
    "negative-divisor": (17, -5, -4, -3),
    "both-negative": (-17, -5, 3, -2),
    "exact-negative": (-20, 4, -5, 0),
    "small-negative": (-3, 10, -1, 7),
    "zero-dividend": (0, 7, 0, 0),
    "zero-negative-divisor": (0, -7, 0, 0),
    "divisor-one": (9, 1, 9, 0),
    "int-max": (2147483647, 2, 1073741823, 1),
    "int-min": (-2147483648, 3, -715827883, 1),
    "min-by-max": (-2147483648, 2147483647, -2, 2147483646),
    "max-by-min": (2147483647, -2147483648, -1, -1),
}


@pytest.mark.parametrize("name", CASES)
def test_div_mod(name):
    a, b, q, r = CASES[name]
    quotient, remainder = div_mod(a, b)
    assert (quotient, remainder) == (q, r)


@pytest.mark.parametrize("name", CASES)
def test_same_as_builtin_divmod(name):
    a, b, _, _ = CASES[name]
    assert div_mod(a, b) == divmod(a, b)
