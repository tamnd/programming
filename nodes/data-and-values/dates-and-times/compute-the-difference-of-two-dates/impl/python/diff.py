# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

from datetime import date


def days_between(
    start_year: int, start_month: int, start_day: int, end_year: int, end_month: int, end_day: int
) -> int:
    """Return the number of days from the start date to the end date."""
    return (date(end_year, end_month, end_day) - date(start_year, start_month, start_day)).days
