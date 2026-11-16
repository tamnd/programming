# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-an-enumeration/.

import pytest

from cycle import Day, cycle

CASES = {
    "weekdays": ("monday", 3, ["monday", "tuesday", "wednesday"]),
    "wrap": ("friday", 4, ["friday", "saturday", "sunday", "monday"]),
    "zero-steps": ("monday", 0, []),
    "more-than-a-week": (
        "sunday",
        9,
        ["sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday", "monday"],
    ),
    "unknown": ("funday", 3, []),
    "wrong-case": ("Friday", 2, []),
    "number": ("3", 2, []),
    "empty-name": ("", 1, []),
}


@pytest.mark.parametrize("case", CASES)
def test_cycle(case):
    start, steps, want = CASES[case]
    assert cycle(start, steps) == want


def test_members():
    assert len(Day) == 7
    assert Day["FRIDAY"] is Day.FRIDAY
    assert Day.FRIDAY.name == "FRIDAY"
    assert Day.FRIDAY.value == "friday"
