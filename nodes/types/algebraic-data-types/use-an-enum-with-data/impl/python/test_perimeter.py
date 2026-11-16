# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-an-enum-with-data/.
import pytest

from perimeter import Rectangle, Square, make_shape, perimeter

CASES = {
    "square": ("square", [3], 12),
    "rectangle": ("rectangle", [2, 5], 14),
    "triangle": ("triangle", [3, 4, 5], 12),
    "unknown-kind": ("circle", [2], None),
    "wrong-case": ("Square", [3], None),
    "too-few-sides": ("rectangle", [4], None),
    "too-many-sides": ("square", [3, 3], None),
    "no-sides": ("triangle", [], None),
    "zero-side": ("square", [0], None),
    "negative-side": ("rectangle", [-2, 5], None),
    "not-a-triangle": ("triangle", [1, 2, 3], None),
    "largest": ("triangle", [1000000, 1000000, 1000000], 3000000),
}


@pytest.mark.parametrize("case", CASES)
def test_perimeter(case):
    kind, sides, want = CASES[case]
    assert perimeter(kind, sides) == want


def test_variants_carry_their_data():
    assert make_shape("square", [3]) == Square(3)
    assert make_shape("rectangle", [2, 5]) == Rectangle(width=2, height=5)
