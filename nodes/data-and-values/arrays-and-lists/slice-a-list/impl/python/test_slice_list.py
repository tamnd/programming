# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/slice-a-list/.

import pytest

from slice_list import slice_list

CASES = {
    "middle": ([1, 2, 3, 4, 5], 1, 3, [2, 3]),
    "prefix": ([1, 2, 3], 0, 2, [1, 2]),
    "suffix": ([1, 2, 3], 1, 3, [2, 3]),
    "empty-list": ([], 0, 0, []),
    "empty-slice": ([1, 2, 3], 2, 2, []),
    "whole": ([1, 2, 3], 0, 3, [1, 2, 3]),
    "single": ([7], 0, 1, [7]),
    "last-one": ([1, 2, 3], 2, 3, [3]),
    "at-end": ([1, 2, 3], 3, 3, []),
    "limit": ([2147483647, -2147483648, 0], 0, 2, [2147483647, -2147483648]),
}


@pytest.mark.parametrize("name", CASES)
def test_slice_list(name):
    items, start, stop, want = CASES[name]
    items0 = list(items)
    got = slice_list(items, start, stop)
    assert got == want
    if got:
        got[0] ^= 1
    assert items == items0


@pytest.mark.parametrize("start, stop", [(0, 4), (2, 1)])
def test_bad_range(start, stop):
    with pytest.raises(IndexError):
        slice_list([1, 2, 3], start, stop)
