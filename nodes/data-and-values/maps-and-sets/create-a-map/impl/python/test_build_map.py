# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-map/.

import pytest

from build_map import build_map

CASES = {
    "fruits": ([("apple", 3), ("pear", 5), ("fig", 2)], [], [("apple", 3), ("fig", 2), ("pear", 5)]),
    "update-and-delete": ([("a", 1), ("b", 2), ("a", 7), ("c", 3)], ["b"], [("a", 7), ("c", 3)]),
    "empty": ([], [], []),
    "replace": ([("k", 1), ("k", 2), ("k", 3)], [], [("k", 3)]),
    "delete-absent": ([("a", 1)], ["z"], [("a", 1)]),
    "delete-all": ([("a", 1), ("b", 2)], ["a", "b"], []),
    "delete-twice": ([("a", 1), ("b", 2)], ["a", "a"], [("b", 2)]),
    "zero-value": ([("a", 0), ("b", -4)], [], [("a", 0), ("b", -4)]),
    "case": ([("b", 1), ("B", 2), ("a", 3), ("A", 4)], [], [("A", 4), ("B", 2), ("a", 3), ("b", 1)]),
}


@pytest.mark.parametrize("name", CASES)
def test_build_map(name):
    pairs, remove, want = CASES[name]
    assert build_map(pairs, remove) == want
