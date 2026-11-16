# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-switch-or-match/.

def days_in_month(month: int) -> int:
    """Return the number of days in a month of a common year, or 0 for a bad month."""
    match month:
        case 2:
            return 28
        case 4 | 6 | 9 | 11:
            return 30
        case 1 | 3 | 5 | 7 | 8 | 10 | 12:
            return 31
        case _:
            return 0
