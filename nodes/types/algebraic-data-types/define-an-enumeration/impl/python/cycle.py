# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-an-enumeration/.

from enum import Enum


class Day(Enum):
    """The days of the week, in order. Each value is the name in lower case."""

    MONDAY = "monday"
    TUESDAY = "tuesday"
    WEDNESDAY = "wednesday"
    THURSDAY = "thursday"
    FRIDAY = "friday"
    SATURDAY = "saturday"
    SUNDAY = "sunday"


def cycle(start: str, steps: int) -> list[str]:
    """Returns steps day names from start on, with monday after sunday."""
    try:
        day = Day(start)  # from a string: look up the member by value
    except ValueError:
        return []
    days = list(Day)  # iteration gives the members in definition order
    i = days.index(day)  # to an integer: the position
    return [days[(i + k) % len(days)].value for k in range(steps)]
