# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-conditional/.

import pytest

from grade import grade

CASES = {
    "ninety-five": (95, "A"),
    "eighty-five": (85, "B"),
    "seventy-five": (75, "C"),
    "sixty-five": (65, "D"),
    "forty": (40, "F"),
    "hundred": (100, "A"),
    "zero": (0, "F"),
    "ninety": (90, "A"),
    "eighty-nine": (89, "B"),
    "sixty": (60, "D"),
    "fifty-nine": (59, "F"),
}


@pytest.mark.parametrize("name", CASES)
def test_grade(name):
    score, want = CASES[name]
    assert grade(score) == want
