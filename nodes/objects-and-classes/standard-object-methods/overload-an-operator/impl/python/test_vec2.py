# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/overload-an-operator/.

import pytest

from vec2 import Vec2, vector_sum

CASES = {
    "two": (([1, 3], [2, 4]), [4, 6]),
    "three": (([1, 2, 3], [10, 20, 30]), [6, 60]),
    "empty": (([], []), [0, 0]),
    "single": (([5], [-7]), [5, -7]),
    "negative": (([-1, -2], [-3, -4]), [-3, -7]),
    "cancel": (([3, -3], [5, -5]), [0, 0]),
    "limit": (([10**9] * 3, [-(10**9)] * 3), [3 * 10**9, -3 * 10**9]),
}


@pytest.mark.parametrize("name", CASES)
def test_vector_sum(name):
    args, want = CASES[name]
    assert vector_sum(*args) == want


def test_operators():
    a, b = Vec2(1, 2), Vec2(3, 4)
    assert a + b == Vec2(4, 6)
    assert a != b and a == Vec2(1, 2)
    assert hash(a) == hash(Vec2(1, 2))
    with pytest.raises(TypeError):
        a + 3
