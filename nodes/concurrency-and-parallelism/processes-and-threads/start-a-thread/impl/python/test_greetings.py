# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/start-a-thread/.

import threading

import pytest

from greetings import thread_greetings

MANY = [f"t{i}" for i in range(32)]

CASES = {
    "two": (["Ada", "Linus"], ["hello from Ada", "hello from Linus"]),
    "three": (["main", "worker", "logger"], ["hello from main", "hello from worker", "hello from logger"]),
    "empty": ([], []),
    "single": (["solo"], ["hello from solo"]),
    "empty-name": ([""], ["hello from "]),
    "duplicate": (["twin", "twin"], ["hello from twin", "hello from twin"]),
    "unicode": (["Zoë", "李雷"], ["hello from Zoë", "hello from 李雷"]),
    "many": (MANY, [f"hello from {n}" for n in MANY]),
}


@pytest.mark.parametrize("name", CASES)
def test_thread_greetings(name):
    names, want = CASES[name]
    assert thread_greetings(names) == want


def test_no_thread_is_left_running():
    before = threading.active_count()
    thread_greetings(MANY)
    assert threading.active_count() == before
