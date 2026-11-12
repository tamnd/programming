# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/generate-random-numbers/.

import re

import pytest

from random_numbers import int_in_range, secure_words, seeded_words, shuffle, token_hex

RANGE_CASES = [
    ("coin", [5, 2, 7], 0, 1, 3, [1, 0, 1]),
    ("no-results", [], 1, 6, 0, []),
    ("die", [7, 12, 1000000, 4294967295], 1, 6, 4, [2, 1, 5, 4]),
    ("negative-range", [100, 2000000000, 4294967294], -10, 10, 3, [6, -5, -8]),
    ("one-value", [0, 99], 5, 5, 2, [5, 5]),
    ("reject-low-words", [0, 3, 4], 1, 6, 1, [5]),
    ("accept-least", [3, 4, 9], 0, 5, 2, [4, 3]),
    ("power-of-two", [0, 4294967295], 0, 7, 2, [0, 7]),
    ("worst-case", [2147483646, 2147483647], 0, 2147483648, 1, [2147483647]),
    ("full-unsigned-32", [0, 4294967295, 123456789], 0, 4294967295, 3, [0, 4294967295, 123456789]),
    ("full-signed-32", [0, 2147483648, 4294967295], -2147483648, 2147483647, 3, [-2147483648, 0, 2147483647]),
    ("near-int64-max", [4294967295, 6], 9223372036854775798, 9223372036854775807, 2, [9223372036854775803, 9223372036854775804]),
    ("near-int64-min", [4294967295, 9], -9223372036854775808, -9223372036854775799, 2, [-9223372036854775803, -9223372036854775799]),
    ("wide-int64", [4294967295, 0], 9223372032559808512, 9223372036854775807, 2, [9223372036854775807, 9223372032559808512]),
]

SHUFFLE_CASES = [
    ("empty", [], [], []),
    ("one-item", [], ["a"], ["a"]),
    ("two-items-swap", [0], ["a", "b"], ["b", "a"]),
    ("two-items-keep", [1], ["a", "b"], ["a", "b"]),
    ("five-items", [7, 3, 4, 1], ["a", "b", "c", "d", "e"], ["a", "e", "b", "d", "c"]),
    ("reject-in-shuffle", [0, 2, 0], ["a", "b", "c"], ["b", "a", "c"]),
    ("duplicates", [1, 1], ["x", "x", "y"], ["x", "y", "x"]),
    ("non-ascii", [4294967295, 4294967294, 0], ["é", "日本", "🙂", "a"], ["日本", "é", "🙂", "a"]),
]


def fake_words(words):
    """Return a word source that gives words in order, and a check that all were used."""
    it = iter(words)
    return (lambda: next(it)), (lambda: next(it, None) is None)


@pytest.mark.parametrize(("name", "words", "lo", "hi", "count", "want"), RANGE_CASES)
def test_int_in_range(name, words, lo, hi, count, want):
    source, used_all = fake_words(words)
    assert [int_in_range(source, lo, hi) for _ in range(count)] == want, name
    assert used_all(), name


@pytest.mark.parametrize(("name", "words", "items", "want"), SHUFFLE_CASES)
def test_shuffle(name, words, items, want):
    source, used_all = fake_words(words)
    shuffle(source, items)
    assert items == want, name
    assert used_all(), name


@pytest.mark.parametrize(("lo", "hi"), [(1, 0), (0, 1 << 32)])
def test_int_in_range_rejects_bad_range(lo, hi):
    with pytest.raises(ValueError):
        int_in_range(lambda: 0, lo, hi)


def test_same_seed_same_words():
    a, b, c = seeded_words(42), seeded_words(42), seeded_words(43)
    first = [a() for _ in range(10)]
    assert first == [b() for _ in range(10)]
    assert first != [c() for _ in range(10)]
    assert all(0 <= w < 1 << 32 for w in first)


def test_secure_words_in_range():
    source = secure_words()
    assert all(1 <= int_in_range(source, 1, 6) <= 6 for _ in range(100))


@pytest.mark.parametrize("nbytes", [0, 1, 16, 32])
def test_token_hex(nbytes):
    token = token_hex(nbytes)
    assert re.fullmatch(f"[0-9a-f]{{{2 * nbytes}}}", token)


def test_tokens_differ():
    assert token_hex(16) != token_hex(16)
