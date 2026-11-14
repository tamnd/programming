# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-over-a-map/.

import pytest

from score_lines import score_lines

CASES = {
    "scores": ({"bob": 7, "amy": 9, "cal": 4}, ["amy=9", "bob=7", "cal=4", "total=20"]),
    "one": ({"x": 1}, ["x=1", "total=1"]),
    "empty": ({}, ["total=0"]),
    "case": ({"b": 1, "B": 2, "a": 3, "A": 4}, ["A=4", "B=2", "a=3", "b=1", "total=10"]),
    "negative": ({"a": -5, "b": 5}, ["a=-5", "b=5", "total=0"]),
    "prefix": ({"abc": 3, "a": 2, "ab": 1}, ["a=2", "ab=1", "abc=3", "total=6"]),
    "reverse": ({"j": 1, "i": 2, "h": 3, "g": 4, "f": 5, "e": 6, "d": 7, "c": 8, "b": 9, "a": 10}, ["a=10", "b=9", "c=8", "d=7", "e=6", "f=5", "g=4", "h=3", "i=2", "j=1", "total=55"]),
}


@pytest.mark.parametrize("name", CASES)
def test_score_lines(name):
    scores, want = CASES[name]
    assert score_lines(scores) == want
