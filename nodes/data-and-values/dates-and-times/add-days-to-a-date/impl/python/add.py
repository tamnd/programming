# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-days-to-a-date/.

import calendar
from datetime import date, timedelta


def add_months(start: date, months: int) -> date:
    """Add months to a date. A day after the end of the new month becomes its last day."""
    total = start.year * 12 + (start.month - 1) + months
    year, month0 = divmod(total, 12)
    last = calendar.monthrange(year, month0 + 1)[1]
    return start.replace(year=year, month=month0 + 1, day=min(start.day, last))


def add_to_date(year: int, month: int, day: int, years: int, months: int, days: int) -> str:
    """Add years and months at once, then days, and return the date as YYYY-MM-DD."""
    moved = add_months(date(year, month, day), years * 12 + months)
    return (moved + timedelta(days=days)).isoformat()
