# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-set-operations/.

import pytest

from compare_sets import compare_sets

CASES = {
    "colors": (["red", "blue", "green"], ["blue", "yellow"], (["blue", "green", "red", "yellow"], ["blue"], ["green", "red"], ["green", "red", "yellow"], False)),
    "subset": (["b"], ["a", "b", "c"], (["a", "b", "c"], ["b"], [], ["a", "c"], True)),
    "both-empty": ([], [], ([], [], [], [], True)),
    "empty-a": ([], ["x"], (["x"], [], [], ["x"], True)),
    "empty-b": (["x"], [], (["x"], [], ["x"], ["x"], False)),
    "equal": (["a", "b"], ["b", "a"], (["a", "b"], ["a", "b"], [], [], True)),
    "disjoint": (["a"], ["b"], (["a", "b"], [], ["a"], ["a", "b"], False)),
    "duplicates": (["a", "a", "b"], ["b", "b"], (["a", "b"], ["b"], ["a"], ["a"], False)),
    "case": (["a"], ["A"], (["A", "a"], [], ["a"], ["A", "a"], False)),
}


@pytest.mark.parametrize("name", CASES)
def test_compare_sets(name):
    a, b, want = CASES[name]
    assert compare_sets(a, b) == want
