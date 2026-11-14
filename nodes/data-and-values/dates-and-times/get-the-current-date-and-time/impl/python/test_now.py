# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-current-date-and-time/.

import re
from datetime import datetime, timezone

import pytest

from now import now_utc, utc_timestamp

CASES = [
    ("epoch", 0, "1970-01-01T00:00:00.000Z"),
    ("billennium", 1000000000000000000, "2001-09-09T01:46:40.000Z"),
    ("with-milliseconds", 1710054000123456789, "2024-03-10T07:00:00.123Z"),
    ("leap-day", 1709208000000000000, "2024-02-29T12:00:00.000Z"),
    ("truncate-not-round", 999999, "1970-01-01T00:00:00.000Z"),
    ("one-ns-before-epoch", -1, "1969-12-31T23:59:59.999Z"),
    ("one-ms-before-epoch", -1000000, "1969-12-31T23:59:59.999Z"),
    ("just-over-one-ms-before-epoch", -1000001, "1969-12-31T23:59:59.998Z"),
    ("last-ms-of-a-year", 1704067199999999999, "2023-12-31T23:59:59.999Z"),
    ("year-2000-is-leap", 951782400000000000, "2000-02-29T00:00:00.000Z"),
    ("year-2100-is-not-leap", 4107542399999999999, "2100-02-28T23:59:59.999Z"),
    ("after-int32-seconds", 2147483648000000000, "2038-01-19T03:14:08.000Z"),
    ("int64-max", 9223372036854775807, "2262-04-11T23:47:16.854Z"),
    ("int64-min", -9223372036854775808, "1677-09-21T00:12:43.145Z"),
]


@pytest.mark.parametrize(("name", "epoch_ns", "want"), CASES, ids=[c[0] for c in CASES])
def test_utc_timestamp(name, epoch_ns, want):
    assert utc_timestamp(epoch_ns) == want


def test_matches_datetime_isoformat():
    moment = datetime(2024, 3, 10, 7, 0, 0, 123000, tzinfo=timezone.utc)
    want = moment.isoformat(timespec="milliseconds").replace("+00:00", "Z")
    assert utc_timestamp(1_710_054_000_123_456_789) == want


def test_now_utc_with_a_fake_clock():
    calls = []

    def clock():
        calls.append("clock")
        return 1_710_054_000_123_456_789

    assert now_utc(clock) == "2024-03-10T07:00:00.123Z"
    assert calls == ["clock"]


def test_now_utc_with_the_real_clock():
    got = now_utc()
    assert re.fullmatch(r"\d{4}-\d\d-\d\dT\d\d:\d\d:\d\d\.\d{3}Z", got)
    assert int(got[:4]) >= 2024
