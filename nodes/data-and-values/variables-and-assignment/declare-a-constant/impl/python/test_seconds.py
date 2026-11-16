# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-constant/.

import pytest

from seconds import SECONDS_PER_DAY, seconds_in

CASES = [
    ("one-day", 1, 86400),
    ("one-week", 7, 604800),
    ("one-year", 365, 31536000),
    ("zero", 0, 0),
    ("two-days", 2, 172800),
    ("leap-year", 366, 31622400),
    ("last-32-bit-day", 24855, 2147472000),
    ("past-32-bits", 24856, 2147558400),
    ("million-days", 1000000, 86400000000),
]


@pytest.mark.parametrize(("name", "days", "want"), CASES, ids=[c[0] for c in CASES])
def test_seconds_in(name, days, want):
    assert seconds_in(days) == want


def test_seconds_per_day():
    assert SECONDS_PER_DAY == 86400
