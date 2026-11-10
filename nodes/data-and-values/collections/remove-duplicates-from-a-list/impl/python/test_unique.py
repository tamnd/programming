# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/remove-duplicates-from-a-list/.

import pytest

from unique import User, unique, unique_by_email

CASES = {
    "repeats": (["apple", "pear", "apple", "fig", "pear"], ["apple", "pear", "fig"]),
    "words": (["to", "be", "or", "not", "to", "be"], ["to", "be", "or", "not"]),
    "empty": ([], []),
    "single": (["a"], ["a"]),
    "all-same": (["x", "x", "x"], ["x"]),
    "no-duplicates": (["c", "a", "b"], ["c", "a", "b"]),
    "not-adjacent": (["a", "b", "a", "b"], ["a", "b"]),
    "adjacent": (["a", "a", "b", "b", "a"], ["a", "b"]),
    "empty-string": (["", "a", "", "b"], ["", "a", "b"]),
    "case": (["a", "A", "a"], ["a", "A"]),
    "normalization": (["\u00e9", "e\u0301", "\u00e9"], ["\u00e9", "e\u0301"]),
    "whitespace": ([" a", "a", "a ", " a"], [" a", "a", "a "]),
    "number-like": (["1", "01", "1.0", "1"], ["1", "01", "1.0"]),
    "non-ascii": (["日本", "🙂", "日本", "🙂", "ü"], ["日本", "🙂", "ü"]),
}

# The cases of cases/by-key.jsonl.
BY_KEY = {
    "case-insensitive": (
        [("Ada", "ada@example.com"), ("Bob", "bob@example.com"), ("Ada L.", "ADA@Example.com")],
        [("Ada", "ada@example.com"), ("Bob", "bob@example.com")],
    ),
    "empty": ([], []),
    "same-name-other-email": (
        [("Sam", "sam@a.example"), ("Sam", "sam@b.example")],
        [("Sam", "sam@a.example"), ("Sam", "sam@b.example")],
    ),
    "first-record-wins": (
        [("Old", "x@example.com"), ("New", "X@EXAMPLE.COM")],
        [("Old", "x@example.com")],
    ),
    "dots-differ": (
        [("Al", "a.b@example.com"), ("Bo", "ab@example.com")],
        [("Al", "a.b@example.com"), ("Bo", "ab@example.com")],
    ),
    "not-adjacent": (
        [("A", "a@example.com"), ("B", "b@example.com"), ("C", "A@example.com"), ("D", "B@EXAMPLE.com")],
        [("A", "a@example.com"), ("B", "b@example.com")],
    ),
}


@pytest.mark.parametrize("name", CASES)
def test_unique(name: str) -> None:
    given, want = CASES[name]
    items = list(given)
    assert unique(items) == want
    assert items == given, "the input must not change"


@pytest.mark.parametrize("name", BY_KEY)
def test_unique_by_email(name: str) -> None:
    given, want = BY_KEY[name]
    assert unique_by_email([User(*u) for u in given]) == [User(*u) for u in want]
