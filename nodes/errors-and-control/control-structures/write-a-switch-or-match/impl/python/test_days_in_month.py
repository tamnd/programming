# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-switch-or-match/.

import pytest

from days_in_month import days_in_month

CASES = {
    "january": (1, 31),
    "february": (2, 28),
    "april": (4, 30),
    "july": (7, 31),
    "november": (11, 30),
    "december": (12, 31),
    "zero": (0, 0),
    "thirteen": (13, 0),
    "minus-one": (-1, 0),
}


@pytest.mark.parametrize("name", CASES)
def test_days_in_month(name):
    month, want = CASES[name]
    assert days_in_month(month) == want
