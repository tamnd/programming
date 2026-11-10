# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/measure-elapsed-time/.

import pytest

from elapsed import elapsed_ms, measure

CASES = [
    ("one-and-a-half-ms", 1000000000, 1001500000, "1.500 ms"),
    ("seconds", 0, 2345678901, "2345.678 ms"),
    ("under-one-ms", 50000, 750000, "0.700 ms"),
    ("zero", 123456789, 123456789, "0.000 ms"),
    ("under-one-microsecond", 0, 999, "0.000 ms"),
    ("truncate-not-round", 0, 1999999, "1.999 ms"),
    ("leading-zero-in-fraction", 0, 1005000, "1.005 ms"),
    ("exact-ms", 0, 7000000, "7.000 ms"),
    ("one-hour", 0, 3600000000000, "3600000.000 ms"),
    ("negative-readings", -5000000, -3500000, "1.500 ms"),
    ("across-zero", -1000000, 500000, "1.500 ms"),
    ("beyond-2-pow-53", 9007199254740993, 9007199255741992, "1.000 ms"),
    ("near-int64-max", 9223372035854775807, 9223372036854775807, "1000.000 ms"),
    ("near-int64-min", -9223372036854775808, -9223372035854775808, "1000.000 ms"),
]


@pytest.mark.parametrize(("name", "start_ns", "end_ns", "want"), CASES, ids=[c[0] for c in CASES])
def test_elapsed_ms(name, start_ns, end_ns, want):
    assert elapsed_ms(start_ns, end_ns) == want


def test_float_seconds_lose_the_difference():
    # The case beyond-2-pow-53 gives another answer with float arithmetic.
    start, end = 9_007_199_254_740_993, 9_007_199_255_741_992
    assert end - start == 1_000_999
    assert int(float(end) - float(start)) == 1_001_000


def test_measure_with_a_fake_clock():
    calls = []
    readings = iter([5_000_000, 6_500_000])

    def clock():
        calls.append("clock")
        return next(readings)

    assert measure(lambda: calls.append("work"), clock) == 1_500_000
    assert calls == ["clock", "work", "clock"]


def test_measure_with_the_monotonic_clock():
    calls = []
    assert measure(lambda: calls.append("work")) >= 0
    assert calls == ["work"]
