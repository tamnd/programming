# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-day-of-the-week/.

from datetime import date


def day_of_week(year: int, month: int, day: int) -> int:
    """Return the ISO 8601 day of the week: 1 for Monday to 7 for Sunday."""
    return date(year, month, day).isoweekday()


def iso_week(year: int, month: int, day: int) -> tuple[int, int]:
    """Return the ISO 8601 week-based year and week number of a date."""
    iso = date(year, month, day).isocalendar()
    return iso.year, iso.week
