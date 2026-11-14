# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-days-to-a-date/.

from datetime import date

import pytest

from add import add_months, add_to_date

CASES = [
    ("one-day", (2024, 3, 10, 0, 0, 1), "2024-03-11"),
    ("one-month", (2024, 1, 15, 0, 1, 0), "2024-02-15"),
    ("one-year", (2023, 6, 1, 1, 0, 0), "2024-06-01"),
    ("day-into-next-year", (2023, 12, 31, 0, 0, 1), "2024-01-01"),
    ("nothing-to-add", (2024, 2, 29, 0, 0, 0), "2024-02-29"),
    ("month-end-to-leap-february", (2024, 1, 31, 0, 1, 0), "2024-02-29"),
    ("month-end-to-short-february", (2023, 1, 31, 0, 1, 0), "2023-02-28"),
    ("month-end-to-30-day-month", (2024, 3, 31, 0, 1, 0), "2024-04-30"),
    ("back-from-month-end", (2024, 5, 31, 0, -1, 0), "2024-04-30"),
    ("back-to-leap-february", (2024, 3, 31, 0, -1, 0), "2024-02-29"),
    ("leap-day-plus-one-year", (2024, 2, 29, 1, 0, 0), "2025-02-28"),
    ("leap-day-plus-four-years", (2024, 2, 29, 4, 0, 0), "2028-02-29"),
    ("year-and-month-at-once", (2024, 2, 29, 1, 1, 0), "2025-03-29"),
    ("months-before-days", (2024, 1, 31, 0, 1, 1), "2024-03-01"),
    ("month-into-next-year", (2023, 12, 15, 0, 1, 0), "2024-01-15"),
    ("thirteen-months-back", (2024, 1, 15, 0, -13, 0), "2022-12-15"),
    ("day-back-into-last-year", (2024, 1, 1, 0, 0, -1), "2023-12-31"),
    ("year-1900-is-not-leap", (1900, 2, 28, 0, 0, 1), "1900-03-01"),
    ("year-2100-is-not-leap", (2096, 2, 29, 4, 0, 0), "2100-02-28"),
    ("many-days", (1970, 1, 1, 0, 0, 2932896), "9999-12-31"),
    ("many-days-back", (9999, 12, 31, 0, 0, -3652058), "0001-01-01"),
    ("many-months", (1, 1, 31, 0, 119987, 0), "9999-12-31"),
]


@pytest.mark.parametrize(("name", "fields", "want"), CASES, ids=[c[0] for c in CASES])
def test_add_to_date(name, fields, want):
    assert add_to_date(*fields) == want


def test_year_then_month_is_not_thirteen_months():
    leap_day = date(2024, 2, 29)
    assert add_months(add_months(leap_day, 12), 1) == date(2025, 3, 28)
    assert add_months(leap_day, 13) == date(2025, 3, 29)


def test_a_month_and_back_can_lose_days():
    assert add_months(add_months(date(2024, 1, 31), 1), -1) == date(2024, 1, 29)
