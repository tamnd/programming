# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rotate-a-list/.

import pytest

from rotate_left import rotate_left

CASES = {
    "by-two": ([1, 2, 3, 4, 5], 2, [3, 4, 5, 1, 2]),
    "by-one": ([1, 2, 3], 1, [2, 3, 1]),
    "zero": ([1, 2, 3], 0, [1, 2, 3]),
    "full-turn": ([1, 2, 3], 3, [1, 2, 3]),
    "more-than-length": ([1, 2, 3], 4, [2, 3, 1]),
    "much-larger": ([1, 2, 3], 2000000000, [3, 1, 2]),
    "empty": ([], 5, []),
    "empty-zero": ([], 0, []),
    "single": ([7], 3, [7]),
    "duplicates": ([1, 1, 2], 1, [1, 2, 1]),
    "negative-items": ([-1, -2, -3], 1, [-2, -3, -1]),
}


@pytest.mark.parametrize("name", CASES)
def test_rotate_left(name):
    items, k, want = CASES[name]
    assert rotate_left(items, k) == want


def test_input_does_not_change():
    items = [1, 2, 3]
    rotate_left(items, 1)
    assert items == [1, 2, 3]
