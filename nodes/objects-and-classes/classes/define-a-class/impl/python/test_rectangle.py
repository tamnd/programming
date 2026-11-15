# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-class/.

import pytest

from rectangle import Rectangle, rectangle_report

CASES = {
    "three-by-four": ((3, 4, 2), [12, 14, 48, 28]),
    "square": ((5, 5, 3), [25, 20, 225, 60]),
    "zero-side": ((0, 7, 2), [0, 14, 0, 28]),
    "zero-factor": ((3, 4, 0), [12, 14, 0, 0]),
    "identity": ((3, 4, 1), [12, 14, 12, 14]),
    "limit": ((1000, 1000, 46), [1000000, 4000, 2116000000, 184000]),
}


@pytest.mark.parametrize("name", CASES)
def test_rectangle_report(name):
    args, want = CASES[name]
    assert rectangle_report(*args) == want


def test_instances_have_their_own_fields():
    a = Rectangle(1, 2)
    b = Rectangle(1, 2)
    a.scale(3)
    assert (a.width, a.height) == (3, 6)
    assert (b.width, b.height) == (1, 2)
    assert repr(a) == "Rectangle(3, 6)"
