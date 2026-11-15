# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-method-on-a-type/.

import dataclasses

import pytest

from point import Point, point_report

CASES = {
    "right-and-up": ((1, 2, 3, 4), [4, 6, 7, 10]),
    "mixed-signs": ((5, -1, -2, 3), [3, 2, 5, 5]),
    "zero-move": ((7, -3, 0, 0), [7, -3, 0, 10]),
    "negative": ((-2, -5, -3, -1), [-5, -6, 4, 11]),
    "origin": ((2, 3, -2, -3), [0, 0, 5, 0]),
    "limit": ((100000000, -100000000, 100000000, -100000000), [200000000, -200000000, 200000000, 400000000]),
}


@pytest.mark.parametrize("name", CASES)
def test_point_report(name):
    args, want = CASES[name]
    assert point_report(*args) == want


def test_translate_returns_another_point():
    p = Point(1, 2)
    q = p.translate(3, 4)
    assert p == Point(1, 2)
    assert q == Point(4, 6)
    with pytest.raises(dataclasses.FrozenInstanceError):
        p.x = 9
