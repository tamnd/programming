# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/map-over-a-list/.

import pytest

from square_all import square_all

CASES = {
    "small": ([1, 2, 3], [1, 4, 9]),
    "tens": ([5, 10], [25, 100]),
    "empty": ([], []),
    "single": ([7], [49]),
    "negative": ([-3, -1], [9, 1]),
    "zero": ([0], [0]),
    "limit": ([46340, -46340], [2147395600, 2147395600]),
    "order": ([3, 1, 2], [9, 1, 4]),
    "duplicates": ([2, 2], [4, 4]),
}


@pytest.mark.parametrize("name", CASES)
def test_square_all(name):
    items, want = CASES[name]
    assert square_all(items) == want
