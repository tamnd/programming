# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-with-a-custom-key/.

import pytest

from sort_by_length import sort_by_length

CASES = {
    "fruits": (["pear", "fig", "apple", "kiwi"], ["fig", "kiwi", "pear", "apple"]),
    "three": (["bb", "a", "ccc"], ["a", "bb", "ccc"]),
    "empty": ([], []),
    "single": (["x"], ["x"]),
    "same-length": (["dog", "cat", "ant"], ["ant", "cat", "dog"]),
    "duplicates": (["b", "a", "b"], ["a", "b", "b"]),
    "sorted": (["a", "bb", "ccc"], ["a", "bb", "ccc"]),
    "reverse": (["ccc", "bb", "a"], ["a", "bb", "ccc"]),
    "long": (["abcdefghij", "abcdefghi", "z"], ["z", "abcdefghi", "abcdefghij"]),
    "prefix": (["ab", "a", "abc", "b"], ["a", "b", "ab", "abc"]),
}


@pytest.mark.parametrize("name", CASES)
def test_sort_by_length(name):
    words, want = CASES[name]
    given = list(words)
    assert sort_by_length(words) == want
    assert words == given
