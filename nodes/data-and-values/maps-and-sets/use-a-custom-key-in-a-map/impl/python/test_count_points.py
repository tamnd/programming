# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

import pytest

from count_points import Point, count_points

CASES = {
    "clicks": ([(1, 2), (3, 4), (1, 2)], [(1, 2, 2), (3, 4, 1)]),
    "swap": ([(2, 1), (1, 2)], [(1, 2, 1), (2, 1, 1)]),
    "empty": ([], []),
    "one": ([(0, 0)], [(0, 0, 1)]),
    "negative": ([(-1, 0), (0, -1), (-1, 0)], [(-1, 0, 2), (0, -1, 1)]),
    "joined-digits": ([(1, 23), (12, 3)], [(1, 23, 1), (12, 3, 1)]),
    "all-same": ([(5, 5), (5, 5), (5, 5)], [(5, 5, 3)]),
}


@pytest.mark.parametrize("name", CASES)
def test_count_points(name):
    points, want = CASES[name]
    assert count_points(points) == want


def test_equal_points_have_equal_hashes():
    assert Point(1, 2) == Point(1, 2)
    assert hash(Point(1, 2)) == hash(Point(1, 2))
