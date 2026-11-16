# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/round-a-number/.
import math

import pytest

from round_half_away import round_half_away

CASES = {
    "down": (2.4, 2),
    "up": (2.6, 3),
    "negative": (-2.6, -3),
    "integer": (7.0, 7),
    "half-even-up": (2.5, 3),
    "half-odd-up": (1.5, 2),
    "half-zero": (0.5, 1),
    "half-negative": (-2.5, -3),
    "half-negative-small": (-0.5, -1),
    "negative-below-half": (-0.4, 0),
    "near-half": (0.49999999999999994, 0),
    "zero": (0.0, 0),
    "large-odd": (4503599627370497.0, 4503599627370497),
    "large-half-negative": (-1000000000000000.5, -1000000000000001),
}


@pytest.mark.parametrize("case", CASES)
def test_round_half_away(case):
    x, want = CASES[case]
    got = round_half_away(x)
    assert type(got) is int
    assert got == want


def test_builtin_round_is_half_even():
    assert (round(0.5), round(1.5), round(2.5), round(-0.5)) == (0, 2, 2, 0)
    assert round(2.675, 2) == 2.67


def test_floor_plus_half_is_wrong():
    assert math.floor(0.49999999999999994 + 0.5) == 1
    assert math.floor(4503599627370497.0 + 0.5) == 4503599627370498
