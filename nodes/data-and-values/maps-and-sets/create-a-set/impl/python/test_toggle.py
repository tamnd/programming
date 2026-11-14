# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-set/.

import pytest

from toggle import toggle

CASES = {
    "colors": (["red", "blue", "red", "green"], ["blue", "green"]),
    "fresh": (["b", "a"], ["a", "b"]),
    "empty": ([], []),
    "two-times": (["a", "a"], []),
    "three-times": (["a", "a", "a"], ["a"]),
    "case": (["a", "A"], ["A", "a"]),
    "prefix": (["ab", "a"], ["a", "ab"]),
}


@pytest.mark.parametrize("name", CASES)
def test_toggle(name):
    words, want = CASES[name]
    assert toggle(words) == want
