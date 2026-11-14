# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-between-time-zones/.

from datetime import datetime
from zoneinfo import ZoneInfo


def convert_time_zone(
    year: int,
    month: int,
    day: int,
    hour: int,
    minute: int,
    second: int,
    from_zone: str,
    to_zone: str,
) -> str:
    """Convert a local date and time in from_zone to local time in to_zone.

    The default fold=0 moves a time in a gap forward by the length of the gap
    and picks the earlier of two instants in an overlap.
    """
    local = datetime(year, month, day, hour, minute, second, tzinfo=ZoneInfo(from_zone))
    return local.astimezone(ZoneInfo(to_zone)).isoformat()
