# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-for-a-leap-year/.

import calendar

import pytest

from leap import is_leap_year, is_leap_year_library

CASES = [
    ("divisible-by-4", 2024, True),
    ("not-divisible-by-4", 2023, False),
    ("century", 1900, False),
    ("divisible-by-400", 2000, True),
    ("next-century", 2100, False),
    ("year-1600", 1600, True),
    ("year-1700", 1700, False),
    ("year-2400", 2400, True),
    ("year-1", 1, False),
    ("year-4", 4, True),
    ("year-100", 100, False),
    ("year-1500-julian-leap", 1500, False),
    ("year-1582", 1582, False),
    ("year-9999", 9999, False),
    ("year-zero", 0, True),
    ("year-minus-1", -1, False),
    ("year-minus-4", -4, True),
    ("year-minus-100", -100, False),
    ("year-minus-400", -400, True),
]


@pytest.mark.parametrize(("name", "year", "want"), CASES, ids=[c[0] for c in CASES])
def test_is_leap_year(name, year, want):
    assert is_leap_year(year) is want
    assert is_leap_year_library(year) is want


def test_same_as_library_for_every_year():
    assert all(is_leap_year(y) == calendar.isleap(y) for y in range(-9999, 10000))


def test_leap_days_in_400_years():
    assert sum(is_leap_year(y) for y in range(2000, 2400)) == 97
