# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/merge-two-maps/.

import pytest

from merge_maps import merge_maps

CASES = {
    "prices": ({"apple": 3, "pear": 5}, {"pear": 6, "fig": 2}, [("apple", 3), ("fig", 2), ("pear", 6)]),
    "disjoint": ({"a": 1}, {"b": 2}, [("a", 1), ("b", 2)]),
    "empty-base": ({}, {"a": 1}, [("a", 1)]),
    "empty-override": ({"a": 1}, {}, [("a", 1)]),
    "both-empty": ({}, {}, []),
    "same-keys": ({"a": 1, "b": 2}, {"a": 9, "b": 8}, [("a", 9), ("b", 8)]),
    "zero-wins": ({"a": 5}, {"a": 0}, [("a", 0)]),
    "case": ({"a": 1}, {"A": 2}, [("A", 2), ("a", 1)]),
}


@pytest.mark.parametrize("name", CASES)
def test_merge_maps(name):
    base, override, want = CASES[name]
    before = (dict(base), dict(override))
    assert merge_maps(base, override) == want
    assert (base, override) == before
