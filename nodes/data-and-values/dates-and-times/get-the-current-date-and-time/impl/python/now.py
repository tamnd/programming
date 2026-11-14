# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-current-date-and-time/.

import time
from collections.abc import Callable
from datetime import datetime, timedelta, timezone

EPOCH = datetime(1970, 1, 1, tzinfo=timezone.utc)


def utc_timestamp(epoch_ns: int) -> str:
    """Return the UTC date and time of epoch_ns as "YYYY-MM-DDTHH:MM:SS.mmmZ"."""
    # Floor division goes toward the past, so -1 ns is the last millisecond of 1969.
    ms = epoch_ns // 1_000_000
    moment = EPOCH + timedelta(milliseconds=ms)
    return f"{moment:%Y-%m-%dT%H:%M:%S}.{ms % 1000:03d}Z"


def now_utc(clock: Callable[[], int] = time.time_ns) -> str:
    """Read the clock once and return the UTC date and time of the reading."""
    # time_ns gives an int, so the nanoseconds are not rounded to a float.
    return utc_timestamp(clock())
