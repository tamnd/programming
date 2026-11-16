# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-lazy-sequences/.

from itertools import count, islice

import pytest

import primes as mod
from primes import first_primes, primes

CASES = {
    "five": (5, [2, 3, 5, 7, 11]),
    "ten": (10, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29]),
    "twenty-five": (25, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]),
    "zero": (0, []),
    "one": (1, [2]),
    "two": (2, [2, 3]),
    "past-a-gap": (31, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127]),
}


@pytest.mark.parametrize("name", CASES)
def test_first_primes(name):
    n, want = CASES[name]
    assert first_primes(n) == want


def test_primes_is_lazy():
    seq = primes()
    assert next(seq) == 2
    assert list(islice(seq, 3)) == [3, 5, 7]


def test_reads_only_what_it_needs(monkeypatch):
    read = []

    def counting(start):
        for k in count(start):
            read.append(k)
            yield k

    monkeypatch.setattr(mod, "count", counting)
    assert first_primes(5) == [2, 3, 5, 7, 11]
    # The pipeline stops at 11, the fifth prime.
    assert read[-1] == 11
