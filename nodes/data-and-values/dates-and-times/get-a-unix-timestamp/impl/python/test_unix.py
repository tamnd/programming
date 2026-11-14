# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-unix-timestamp/.

from datetime import datetime, timedelta, timezone

import pytest

from unix import now_unix_millis, now_unix_seconds, unix_timestamp

CASES = [
    ("epoch", (1970, 1, 1, 0, 0, 0, 0), 0),
    ("billennium", (2001, 9, 9, 1, 46, 40, 0), 1000000000),
    ("negative-offset", (2024, 3, 10, 2, 0, 0, -300), 1710054000),
    ("positive-offset", (2024, 3, 10, 16, 0, 0, 540), 1710054000),
    ("one-second-before-epoch", (1969, 12, 31, 23, 59, 59, 0), -1),
    ("offset-moves-before-epoch", (1970, 1, 1, 0, 0, 0, 60), -3600),
    ("offset-moves-to-next-year", (2023, 12, 31, 19, 0, 0, -300), 1704067200),
    ("offset-of-45-minutes", (2024, 1, 1, 5, 45, 0, 345), 1704067200),
    ("offset-of-minus-9-30", (2023, 12, 31, 14, 30, 0, -570), 1704067200),
    ("offset-plus-14", (1970, 1, 1, 14, 0, 0, 840), 0),
    ("offset-minus-12", (1969, 12, 31, 12, 0, 0, -720), 0),
    ("leap-day", (2024, 2, 29, 0, 0, 0, 0), 1709164800),
    ("year-2000-is-leap", (2000, 2, 29, 0, 0, 0, 0), 951782400),
    ("year-1900-is-not-leap", (1900, 3, 1, 0, 0, 0, 0), -2203891200),
    ("int32-max", (2038, 1, 19, 3, 14, 7, 0), 2147483647),
    ("after-int32-max", (2038, 1, 19, 3, 14, 8, 0), 2147483648),
    ("int32-min", (1901, 12, 13, 20, 45, 52, 0), -2147483648),
    ("year-99-is-not-1999", (99, 12, 31, 23, 59, 59, 0), -59011459201),
    ("first-second-of-year-1", (1, 1, 1, 0, 0, 0, 0), -62135596800),
    ("last-second-of-year-9999", (9999, 12, 31, 23, 59, 59, 0), 253402300799),
]


@pytest.mark.parametrize(("name", "fields", "want"), CASES, ids=[c[0] for c in CASES])
def test_unix_timestamp(name, fields, want):
    assert unix_timestamp(*fields) == want


@pytest.mark.parametrize(("name", "fields", "want"), CASES, ids=[c[0] for c in CASES])
def test_back_to_a_date(name, fields, want):
    *date, offset = fields
    zone = timezone(timedelta(minutes=offset))
    assert datetime.fromtimestamp(0, timezone.utc) + timedelta(seconds=want) == datetime(*date, tzinfo=zone)


def test_now():
    seconds = now_unix_seconds()
    millis = now_unix_millis()
    assert seconds >= 1_704_067_200
    assert 0 <= millis // 1000 - seconds <= 1
    assert datetime.fromtimestamp(seconds, timezone.utc).year >= 2024
