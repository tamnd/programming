# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-length-of-an-array/.

import pytest

from length import length

CASES = {
    "three": (["a", "b", "c"], 3),
    "two-words": (["apple", "banana"], 2),
    "empty": ([], 0),
    "single": (["x"], 1),
    "empty-strings": (["", ""], 2),
    "non-ascii": (["é", "日本語"], 2),
    "duplicates": (["a", "a", "a"], 3),
    "ten": (["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"], 10),
}


@pytest.mark.parametrize("name", CASES)
def test_length(name):
    items, want = CASES[name]
    assert length(items) == want
