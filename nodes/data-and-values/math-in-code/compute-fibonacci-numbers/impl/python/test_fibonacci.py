# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

import pytest

from fibonacci import fibonacci, fibonacci_cached

CASES = [
    ("ten", 10, 55),
    ("twenty", 20, 6765),
    ("thirty", 30, 832040),
    ("zero", 0, 0),
    ("one", 1, 1),
    ("two", 2, 1),
    ("largest-32-bit", 46, 1836311903),
    ("beyond-32-bit", 47, 2971215073),
    ("binet-fails", 71, 308061521170129),
    ("largest-64-bit", 92, 7540113804746346429),
]


@pytest.mark.parametrize(("name", "n", "want"), CASES, ids=[c[0] for c in CASES])
def test_fibonacci(name, n, want):
    assert fibonacci(n) == want
    assert fibonacci_cached(n) == want


def test_big():
    assert fibonacci(100) == 354224848179261915075


def test_negative_raises():
    with pytest.raises(ValueError):
        fibonacci(-1)
