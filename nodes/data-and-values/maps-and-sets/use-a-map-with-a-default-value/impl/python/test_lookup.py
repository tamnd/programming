# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

import pytest

from lookup import lookup

CASES = {
    "prices": ({"apple": 3, "pear": 5}, ["apple", "fig", "pear"], 0, [3, 0, 5]),
    "negative-fallback": ({"a": 1}, ["b", "a"], -1, [-1, 1]),
    "empty-table": ({}, ["a", "b"], 7, [7, 7]),
    "empty-keys": ({"a": 1}, [], 0, []),
    "zero-value": ({"a": 0}, ["a", "b"], 5, [0, 5]),
    "repeat": ({"a": 1}, ["b", "b", "a"], 2, [2, 2, 1]),
    "case": ({"a": 1}, ["A"], 0, [0]),
}


@pytest.mark.parametrize("name", CASES)
def test_lookup(name):
    table, keys, fallback, want = CASES[name]
    before = dict(table)
    assert lookup(table, keys, fallback) == want
    assert table == before
