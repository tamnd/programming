# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-day-of-the-week/.

from datetime import date

import pytest

from weekday import day_of_week, iso_week

CASES = [
    ("a-sunday", (2024, 3, 10), 7),
    ("a-monday", (2024, 3, 11), 1),
    ("the-epoch", (1970, 1, 1), 4),
    ("year-2000", (2000, 1, 1), 6),
    ("a-saturday", (2024, 3, 16), 6),
    ("leap-day", (2024, 2, 29), 4),
    ("after-leap-day-2000", (2000, 3, 1), 3),
    ("year-1900-is-not-leap", (1900, 3, 1), 4),
    ("year-2100-is-not-leap", (2100, 3, 1), 1),
    ("before-the-epoch", (1969, 12, 31), 3),
    ("gregorian-reform", (1582, 10, 15), 5),
    ("day-before-the-reform", (1582, 10, 4), 1),
    ("first-day", (1, 1, 1), 1),
    ("last-day", (9999, 12, 31), 5),
    ("year-99", (99, 12, 31), 4),
    ("iso-week-of-next-year", (2024, 12, 30), 1),
    ("iso-week-of-last-year", (2027, 1, 1), 5),
    ("month-end", (2024, 4, 30), 2),
]

WEEKS = [
    ((2024, 3, 10), (2024, 10)),
    ((2024, 12, 30), (2025, 1)),
    ((2027, 1, 1), (2026, 53)),
    ((2000, 1, 1), (1999, 52)),
    ((1969, 12, 31), (1970, 1)),
    ((99, 12, 31), (99, 53)),
]


@pytest.mark.parametrize(("name", "fields", "want"), CASES, ids=[c[0] for c in CASES])
def test_day_of_week(name, fields, want):
    assert day_of_week(*fields) == want


@pytest.mark.parametrize(("fields", "want"), WEEKS)
def test_iso_week(fields, want):
    assert iso_week(*fields) == want


def test_weekday_counts_from_zero():
    sunday = date(2024, 3, 10)
    assert sunday.weekday() == 6
    assert sunday.isoweekday() == 7
