# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-while-loop/.

import pytest

from halvings import halvings

CASES = {
    "rosetta": (1024, [1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1]),
    "ten": (10, [10, 5, 2, 1]),
    "zero": (0, []),
    "one": (1, [1]),
    "odd": (7, [7, 3, 1]),
    "limit": (2147483647, [2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1]),
}


@pytest.mark.parametrize("name", CASES)
def test_halvings(name):
    n, want = CASES[name]
    assert halvings(n) == want
