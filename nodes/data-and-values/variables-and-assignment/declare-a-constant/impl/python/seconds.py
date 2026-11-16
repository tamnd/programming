# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-constant/.

from typing import Final

# Final tells a type checker that the name gets no new value. Python itself
# does not check it, and the upper case name is the convention for a
# constant.
SECONDS_PER_MINUTE: Final = 60
MINUTES_PER_HOUR: Final = 60
HOURS_PER_DAY: Final = 24
SECONDS_PER_DAY: Final = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY


def seconds_in(days: int) -> int:
    """Return the number of seconds in days days of 24 hours."""
    return days * SECONDS_PER_DAY


if __name__ == "__main__":
    print(seconds_in(7))
