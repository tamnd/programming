# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-unix-timestamp/.

import time
from datetime import datetime, timedelta, timezone

EPOCH = datetime(1970, 1, 1, tzinfo=timezone.utc)


def unix_timestamp(
    year: int, month: int, day: int, hour: int, minute: int, second: int, offset_minutes: int
) -> int:
    """Return the seconds since 1970-01-01T00:00:00Z of a local time with the given UTC offset."""
    zone = timezone(timedelta(minutes=offset_minutes))
    moment = datetime(year, month, day, hour, minute, second, tzinfo=zone)
    # Integer division of two timedeltas gives an int. moment.timestamp() gives a float.
    return (moment - EPOCH) // timedelta(seconds=1)


def now_unix_seconds() -> int:
    """Return the current Unix time in whole seconds, rounded toward the past."""
    return time.time_ns() // 1_000_000_000


def now_unix_millis() -> int:
    """Return the current Unix time in whole milliseconds, rounded toward the past."""
    return time.time_ns() // 1_000_000
