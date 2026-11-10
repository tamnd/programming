# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/measure-elapsed-time/.

import time
from collections.abc import Callable


def measure(work: Callable[[], object], clock: Callable[[], int] = time.monotonic_ns) -> int:
    """Run work once and return the time it took, in nanoseconds of clock."""
    # monotonic_ns is not changed by updates of the system clock, and an int has no rounding.
    start = clock()
    work()
    return clock() - start


def elapsed_ms(start_ns: int, end_ns: int) -> str:
    """Return end_ns - start_ns as "M.UUU ms", with the rest under one microsecond dropped."""
    ns = end_ns - start_ns
    return f"{ns // 1_000_000}.{ns // 1_000 % 1_000:03d} ms"
