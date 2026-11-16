# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compose-functions/.

import pytest

from compose import compose, compose_all

CASES = {
    "inc-after-double": (["inc", "double"], 3, 7),
    "double-after-inc": (["double", "inc"], 3, 8),
    "three-functions": (["negate", "dec", "double"], 5, -9),
    "empty": ([], 42, 42),
    "single-zero": (["negate"], 0, 0),
    "negative": (["dec", "negate"], -5, 4),
    "low": (["inc"], -1000, -999),
    "duplicates": (["inc", "inc", "inc"], 7, 10),
    "limit-high": (["double"] * 20, 1000, 1048576000),
    "limit-low": (["double"] * 20, -1000, -1048576000),
    "mixed-20": (["double", "inc"] * 10, 1000, 1026046),
    "mixed-20-swapped": (["inc", "double"] * 10, 1000, 1025023),
}


@pytest.mark.parametrize("name", CASES)
def test_compose_all(name):
    ops, x, want = CASES[name]
    assert compose_all(ops, x) == want


def test_compose_is_generic():
    shout = compose(lambda s: s + "!", str.upper, str.strip)
    assert shout("  hello ") == "HELLO!"


def test_compose_of_nothing_is_identity():
    assert compose()("same") == "same"
