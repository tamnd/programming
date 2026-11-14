# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sleep-for-a-duration/.

import asyncio
import time

import pytest

from sleep import sleep_for, sleep_for_async, sleep_requests

CASES = [
    ("one-call", 1000000, 0, [1000000], [1000000]),
    ("woke-late", 1000000, 0, [1080000], [1000000]),
    ("woke-early", 1000000, 0, [600000, 1000000], [1000000, 400000]),
    ("one-second", 1000000000, 5000000000, [6000050000], [1000000000]),
    ("zero", 0, 0, [], []),
    ("negative", -5000000, 0, [], []),
    ("most-negative", -4611686018427387904, 0, [], []),
    ("several-early-wakes", 10000000, 0, [2000000, 5000000, 9999999, 10000000], [10000000, 8000000, 5000000, 1]),
    ("no-progress", 1000000, 0, [0, 1000000], [1000000, 1000000]),
    ("exact-deadline", 3000000, 100, [3000100], [3000000]),
    ("one-ns-short", 3000000, 0, [2999999, 3000000], [3000000, 1]),
    ("extra-readings", 1000000, 0, [1000000, 5000000], [1000000]),
    ("negative-origin", 2000000000, -5000000000, [-4000000000, -3000000000], [2000000000, 1000000000]),
    ("origin-after-100-days", 1500000, 8640000000000000, [8640000001000000, 8640000001500000], [1500000, 500000]),
    ("one-nanosecond", 1, 0, [1000000], [1]),
    ("sub-millisecond", 1500000, 0, [1000000, 1500000], [1500000, 500000]),
    ("one-hour", 3600000000000, 0, [3600000000000], [3600000000000]),
    ("thirty-days", 2592000000000000, 0, [2147483647000000, 2592000000000000], [2592000000000000, 444516353000000]),
]


@pytest.mark.parametrize(("name", "duration_ns", "start_ns", "readings", "want"), CASES, ids=[c[0] for c in CASES])
def test_sleep_requests(name, duration_ns, start_ns, readings, want):
    assert sleep_requests(duration_ns, start_ns, readings) == want


def test_sleep_for_waits_at_least_the_duration():
    start = time.monotonic_ns()
    sleep_for(20_000_000)
    assert time.monotonic_ns() - start >= 20_000_000


@pytest.mark.parametrize("duration_ns", [0, -5_000_000])
def test_sleep_for_returns_at_once(duration_ns):
    start = time.monotonic_ns()
    sleep_for(duration_ns)
    assert time.monotonic_ns() - start < 1_000_000_000


def test_sleep_for_async_lets_other_tasks_run():
    async def main():
        start = time.monotonic_ns()
        await asyncio.gather(sleep_for_async(100_000_000), sleep_for_async(100_000_000))
        return time.monotonic_ns() - start

    elapsed = asyncio.run(main())
    assert 100_000_000 <= elapsed < 200_000_000
