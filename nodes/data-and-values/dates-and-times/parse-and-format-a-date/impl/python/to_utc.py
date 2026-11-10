# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-and-format-a-date/.

import re
from datetime import datetime, timezone

# The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
# fromisoformat accepts more than this (24:00, +0700, a space for T), so the
# pattern checks the syntax first. [0-9] is ASCII only; \d also matches other
# digits.
RFC3339 = re.compile(
    r"[0-9]{4}-[0-9]{2}-[0-9]{2}[Tt]"
    r"([01][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9](\.[0-9]{1,3})?"
    r"([Zz]|[+-]([01][0-9]|2[0-3]):[0-5][0-9])"
)


def to_utc(timestamp: str) -> str:
    """Return the instant of an RFC 3339 timestamp in UTC, as YYYY-MM-DDTHH:MM:SS.sssZ.

    Raise ValueError when timestamp is not an RFC 3339 date-time with an
    offset, or when its date is not in the calendar.
    """
    if RFC3339.fullmatch(timestamp) is None:
        raise ValueError(f"not an RFC 3339 date-time: {timestamp!r}")
    # The pattern makes sure that there is an offset, so the value is aware.
    # fromisoformat checks the calendar: 2026-02-29 raises ValueError.
    value = datetime.fromisoformat(timestamp.upper())
    text = value.astimezone(timezone.utc).isoformat(timespec="milliseconds")
    return text.removesuffix("+00:00") + "Z"
