# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

import pytest

from prime import is_prime

CASES = [
    ("small-prime", 17, True),
    ("small-composite", 15, False),
    ("even", 100, False),
    ("odd-composite", 91, False),
    ("negative", -7, False),
    ("zero", 0, False),
    ("one", 1, False),
    ("two", 2, True),
    ("three", 3, True),
    ("square-of-prime", 25, False),
    ("large-square", 4293001441, False),
    ("carmichael", 561, False),
    ("large-prime", 1000000007, True),
    ("mersenne-prime", 2147483647, True),
    ("large-composite", 2147483649, False),
]


@pytest.mark.parametrize(("name", "n", "want"), CASES, ids=[c[0] for c in CASES])
def test_is_prime(name, n, want):
    assert is_prime(n) is want


def test_primes_below_30():
    assert [n for n in range(30) if is_prime(n)] == [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]
