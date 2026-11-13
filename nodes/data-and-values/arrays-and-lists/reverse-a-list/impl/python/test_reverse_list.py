# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reverse-a-list/.

import pytest

from reverse_list import reverse_list

CASES = {
    "three": ([1, 2, 3], [3, 2, 1]),
    "four": ([4, 7, 1, 9], [9, 1, 7, 4]),
    "empty": ([], []),
    "single": ([5], [5]),
    "two": ([1, 2], [2, 1]),
    "duplicates": ([1, 1, 2], [2, 1, 1]),
    "palindrome": ([1, 2, 1], [1, 2, 1]),
    "negative": ([-1, 0, -3], [-3, 0, -1]),
    "limit": ([2147483647, -2147483648, 0], [0, -2147483648, 2147483647]),
}


@pytest.mark.parametrize("name", CASES)
def test_reverse_list(name):
    items, want = CASES[name]
    items0 = list(items)
    got = reverse_list(items)
    assert got == want
    assert got is not items
    assert items == items0
