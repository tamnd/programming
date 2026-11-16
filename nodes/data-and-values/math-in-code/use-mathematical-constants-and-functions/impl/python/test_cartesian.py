# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

import math

import pytest

from cartesian import to_cartesian

CASES = [
    ("east", 1.0, 0.0, (1.0, 0.0)),
    ("north", 2.0, 90.0, (0.0, 2.0)),
    ("west", 2.0, 180.0, (-2.0, 0.0)),
    ("diagonal", 1.0, 45.0, (0.7071067811865476, 0.7071067811865476)),
    ("thirty-degrees", 2.0, 30.0, (1.7320508075688772, 1.0)),
    ("zero-radius", 0.0, 123.0, (0.0, 0.0)),
    ("south", 1.0, 270.0, (0.0, -1.0)),
    ("negative-angle", 1.0, -90.0, (0.0, -1.0)),
    ("full-turn", 1.0, 360.0, (1.0, 0.0)),
    ("two-turns", 3.0, 720.0, (3.0, 0.0)),
    ("negative-radius", -1.0, 0.0, (-1.0, 0.0)),
    ("large-radius", 1000000.0, 60.0, (500000.0, 866025.4037844386)),
]


def close(got, want):
    return math.isclose(got, want, rel_tol=1e-12, abs_tol=1e-9)


@pytest.mark.parametrize(("name", "r", "degrees", "want"), CASES, ids=[c[0] for c in CASES])
def test_to_cartesian(name, r, degrees, want):
    x, y = to_cartesian(r, degrees)
    assert close(x, want[0]) and close(y, want[1])


def test_constants():
    assert math.isclose(math.atan2(0, -1), math.pi)
    assert math.isclose(math.log(math.e), 1)
    assert math.sqrt(16) == 4
    assert 2**10 == 1024
