# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/implement-equality-for-a-type/.

import pytest

from money import Money, count_distinct

CASES = {
    "two-equal": (([1250, 1250], ["EUR", "EUR"]), 1),
    "different-amounts": (([100, 200, 100], ["USD", "USD", "USD"]), 2),
    "mixed": (([500, 500, 500, 700], ["EUR", "USD", "EUR", "EUR"]), 3),
    "empty": (([], []), 0),
    "case": (([999, 999, 999], ["gbp", "GBP", "Gbp"]), 1),
    "other-currency": (([0, 0, 0], ["EUR", "USD", "JPY"]), 3),
    "negative": (([-5, 5, -5], ["EUR", "EUR", "EUR"]), 2),
    "limit": (([10**12, -(10**12), 10**12], ["usd", "USD", "USD"]), 2),
}


@pytest.mark.parametrize("name", CASES)
def test_count_distinct(name):
    args, want = CASES[name]
    assert count_distinct(*args) == want


def test_equality_rules():
    a, b, c = Money(1250, "EUR"), Money(1250, "eur"), Money(1250, "EUR")
    assert a == a
    assert a == b and b == a
    assert a == b and b == c and a == c
    assert a != Money(1251, "EUR")
    assert a != Money(1250, "USD")
    assert hash(a) == hash(b)


def test_other_types_are_not_equal():
    assert Money(1, "EUR") != 1
    assert Money(1, "EUR") != (1, "EUR")
    assert Money(1, "EUR") is not None


def test_dict_key():
    names = {Money(500, "USD"): "lunch"}
    assert names[Money(500, "usd")] == "lunch"
