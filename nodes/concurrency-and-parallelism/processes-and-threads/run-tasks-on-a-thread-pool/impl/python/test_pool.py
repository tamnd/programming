# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

import threading

import pytest

from pool import collatz_steps, steps_on_pool


def sequential(numbers):
    return [collatz_steps(n) for n in numbers]


CASES = {
    "four-jobs": (2, [1, 2, 3, 6], [0, 1, 7, 8]),
    "long-runs": (3, [27, 97, 871], [111, 118, 178]),
    "empty": (2, [], []),
    "one-worker": (1, [7, 8, 9], [16, 3, 19]),
    "more-workers-than-jobs": (8, [5, 6], [5, 8]),
    "duplicates": (3, [27, 27, 27, 1], [111, 111, 111, 0]),
    "wide-intermediate": (2, [113383, 837799], [247, 524]),
    "many": (4, list(range(1, 201)), sequential(range(1, 201))),
}


@pytest.mark.parametrize("name", CASES)
def test_steps_on_pool(name):
    workers, numbers, want = CASES[name]
    assert steps_on_pool(workers, numbers) == want


def test_pool_threads_have_stopped():
    before = threading.active_count()
    steps_on_pool(4, list(range(1, 101)))
    assert threading.active_count() == before
