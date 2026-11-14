# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sleep-for-a-duration/.

import asyncio
import time
from collections.abc import Callable


def sleep_loop(duration_ns: int, clock: Callable[[], int], sleep: Callable[[int], None]) -> None:
    """Sleep until clock() reaches the deadline. A sleep that ends early is
    followed by a sleep for the rest of the time."""
    now = clock()
    deadline = now + duration_ns
    while now < deadline:
        sleep(deadline - now)
        now = clock()


def sleep_for(duration_ns: int) -> None:
    """Block the thread for at least duration_ns nanoseconds."""
    sleep_loop(duration_ns, time.monotonic_ns, lambda ns: time.sleep(ns / 1e9))


async def sleep_for_async(duration_ns: int) -> None:
    """Suspend the task for at least duration_ns nanoseconds. Other tasks run
    in the meantime."""
    now = time.monotonic_ns()
    deadline = now + duration_ns
    while now < deadline:
        await asyncio.sleep((deadline - now) / 1e9)
        now = time.monotonic_ns()


def sleep_requests(duration_ns: int, start_ns: int, readings: list[int]) -> list[int]:
    """Run sleep_loop on a fake clock and return the durations it asks for."""
    times = iter([start_ns, *readings])
    requests: list[int] = []
    sleep_loop(duration_ns, lambda: next(times), requests.append)
    return requests
