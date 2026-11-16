# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-singleton/.

import threading

import pytest

from id_generator import IdGenerator, singleton_report

CASES = {
    "three-calls": (3, [2, 1, 1]),
    "ten-calls": (10, [9, 1, 1]),
    "single-call": (1, [0, 1, 1]),
    "pair": (2, [1, 1, 1]),
    "limit": (1000000, [999999, 1, 1]),
}


@pytest.mark.parametrize("name", CASES)
def test_singleton_report(name):
    calls, want = CASES[name]
    assert singleton_report(calls) == want


def test_threads_share_one_instance():
    got = []
    threads = [threading.Thread(target=lambda: got.append(IdGenerator.instance())) for _ in range(8)]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    assert all(g is got[0] for g in got)
    assert IdGenerator.constructions == 1
