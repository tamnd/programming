# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-counted-loop/.

import pytest

from count_to import count_to

CASES = {
    "ten": (10, [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]),
    "three": (3, [1, 2, 3]),
    "zero": (0, []),
    "one": (1, [1]),
    "two": (2, [1, 2]),
}


@pytest.mark.parametrize("name", CASES)
def test_count_to(name):
    n, want = CASES[name]
    assert count_to(n) == want
