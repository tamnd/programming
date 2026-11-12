# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/join-a-list-of-strings/.

import pytest

from join import join, join_numbers

CASES = {
    "comma": (["a", "b", "c"], ",", "a,b,c"),
    "comma-space": (["red", "green", "blue"], ", ", "red, green, blue"),
    "words": (["Hello", "world"], " ", "Hello world"),
    "empty-list": ([], ",", ""),
    "single": (["only"], ",", "only"),
    "empty-items": (["", "", ""], ",", ",,"),
    "empty-middle": (["a", "", "b"], "-", "a--b"),
    "empty-sep": (["a", "b", "c"], "", "abc"),
    "long-sep": (["x", "y"], " -> ", "x -> y"),
    "non-ascii": (["naïve", "日本"], "·", "naïve·日本"),
    "astral-sep": (["a", "b"], "\U0001f642", "a\U0001f642b"),
    "sep-in-item": (["a,b", "c"], ",", "a,b,c"),
}

NUMBER_CASES = {
    "numbers": ([1, 2, 3], ",", "1,2,3"),
    "negative": ([-1, 0, 1], " ", "-1 0 1"),
    "empty": ([], ",", ""),
    "single": ([42], ",", "42"),
    "int32-ends": ([2147483647, -2147483648], ";", "2147483647;-2147483648"),
}


@pytest.mark.parametrize("name", CASES)
def test_join(name):
    items, sep, want = CASES[name]
    assert join(items, sep) == want


@pytest.mark.parametrize("name", NUMBER_CASES)
def test_join_numbers(name):
    items, sep, want = NUMBER_CASES[name]
    assert join_numbers(items, sep) == want
