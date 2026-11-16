# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-higher-order-function/.

import pytest

from repeat import collatz_after, collatz_step, repeat

CASES = {
    "one-step": (6, 1, 3),
    "three-steps": (6, 3, 5),
    "ten-steps": (27, 10, 214),
    "zero-times": (7, 0, 7),
    "start-one": (1, 1, 4),
    "cycle": (1, 3, 1),
    "past-one": (16, 10, 1),
    "peak-27": (27, 77, 9232),
    "peak-9663": (9663, 48, 27114424),
    "limit": (10000, 1000, 2),
}


@pytest.mark.parametrize("name", CASES)
def test_collatz_after(name):
    start, times, want = CASES[name]
    assert collatz_after(start, times) == want


def test_repeat_takes_a_lambda():
    assert repeat(lambda x: x * 2, 10)(1) == 1024


def test_repeat_zero_is_identity():
    assert repeat(collatz_step, 0)(27) == 27


def test_each_call_of_repeat_makes_a_new_function():
    twice = repeat(collatz_step, 2)
    three_times = repeat(collatz_step, 3)
    assert (twice(6), three_times(6)) == (10, 5)
