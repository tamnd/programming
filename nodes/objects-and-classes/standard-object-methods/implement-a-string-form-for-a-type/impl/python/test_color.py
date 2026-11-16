# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

import pytest

from color import Color, format_colors

CASES = {
    "orange": (([255], [128], [0]), ["#FF8000"]),
    "three": (([255, 0, 0], [0, 255, 0], [0, 0, 255]), ["#FF0000", "#00FF00", "#0000FF"]),
    "mixed": (([18, 171], [52, 205], [86, 239]), ["#123456", "#ABCDEF"]),
    "empty": (([], [], []), []),
    "single": (([1], [2], [3]), ["#010203"]),
    "zero": (([0], [0], [0]), ["#000000"]),
    "max": (([255], [255], [255]), ["#FFFFFF"]),
    "padding": (([10, 0], [11, 15], [12, 16]), ["#0A0B0C", "#000F10"]),
}


@pytest.mark.parametrize("name", CASES)
def test_format_colors(name):
    args, want = CASES[name]
    assert format_colors(*args) == want


def test_forms():
    c = Color(255, 128, 0)
    assert str(c) == "#FF8000" and f"{c}" == "#FF8000"
    assert repr(c) == "Color(r=255, g=128, b=0)"
    assert str([c]) == "[Color(r=255, g=128, b=0)]"


def test_bad_value():
    with pytest.raises(ValueError):
        Color(256, 0, 0)
