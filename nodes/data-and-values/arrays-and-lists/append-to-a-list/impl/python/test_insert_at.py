# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-list/.

import pytest

from insert_at import insert_at

CASES = {
    "at-end": ([1, 2], 2, 3, [1, 2, 3]),
    "at-front": ([2, 3], 0, 1, [1, 2, 3]),
    "in-middle": ([1, 3], 1, 2, [1, 2, 3]),
    "empty": ([], 0, 7, [7]),
    "single-front": ([5], 0, 4, [4, 5]),
    "single-end": ([5], 1, 6, [5, 6]),
    "end-of-many": ([1, 2, 3, 4], 4, 5, [1, 2, 3, 4, 5]),
    "front-of-many": ([1, 2, 3, 4], 0, 0, [0, 1, 2, 3, 4]),
    "duplicates": ([1, 1], 1, 1, [1, 1, 1]),
    "limit": ([2147483647], 1, -2147483648, [2147483647, -2147483648]),
}


@pytest.mark.parametrize("name", CASES)
def test_insert_at(name):
    items, index, value, want = CASES[name]
    items0 = list(items)
    assert insert_at(items, index, value) == want
    assert items == items0


def test_index_past_the_end():
    with pytest.raises(IndexError):
        insert_at([1, 2], 3, 9)
