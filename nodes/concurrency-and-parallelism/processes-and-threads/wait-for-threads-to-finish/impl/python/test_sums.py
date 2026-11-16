# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

import threading

import pytest

from sums import parallel_sums

CASES = {
    "two": ([[1, 2, 3], [4, 5]], [6, 9]),
    "three": ([[10], [20, 30], [-5, 5]], [10, 50, 0]),
    "empty": ([], []),
    "empty-chunk": ([[], [7]], [0, 7]),
    "single": ([[42]], [42]),
    "negative": ([[-1, -2], [-3]], [-3, -3]),
    "limit": ([[10**9] * 3, [-(10**9)] * 3], [3 * 10**9, -3 * 10**9]),
    "many": ([[i, i, i] for i in range(32)], [3 * i for i in range(32)]),
}


@pytest.mark.parametrize("name", CASES)
def test_parallel_sums(name):
    chunks, want = CASES[name]
    assert parallel_sums(chunks) == want


def test_all_threads_have_finished():
    before = threading.active_count()
    parallel_sums([[i] for i in range(8)])
    assert threading.active_count() == before
