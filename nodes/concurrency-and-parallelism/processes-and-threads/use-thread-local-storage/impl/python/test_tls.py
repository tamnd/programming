# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-thread-local-storage/.

import threading

import pytest

from tls import bump, count_per_thread, current

CASES = {
    "three-threads": [1, 2, 3],
    "uneven": [10, 0, 5, 1000],
    "empty": [],
    "zero": [0],
    "one-thread": [7],
    "equal-counts": [100, 100, 100, 100],
    "many-threads": list(range(1, 17)),
    "large": [100000, 100000],
}


@pytest.mark.parametrize("name", CASES)
def test_count_per_thread(name):
    counts = CASES[name]
    assert count_per_thread(counts) == counts


def test_caller_counter_does_not_change():
    # Run in a new thread, so that other tests on the main thread do not matter.
    seen = []

    def caller():
        for _ in range(3):
            bump()
        count_per_thread([5, 6])
        seen.append(current())

    t = threading.Thread(target=caller)
    t.start()
    t.join()
    assert seen == [3]
