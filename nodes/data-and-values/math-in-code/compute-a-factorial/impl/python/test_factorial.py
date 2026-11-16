# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-factorial/.

import pytest

from factorial import factorial, factorial_loop, factorial_recursive

CASES = [
    ("five", 5, 120),
    ("ten", 10, 3628800),
    ("zero", 0, 1),
    ("one", 1, 1),
    ("largest-32-bit", 12, 479001600),
    ("beyond-32-bit", 13, 6227020800),
    ("largest-64-bit", 20, 2432902008176640000),
]


@pytest.mark.parametrize(("name", "n", "want"), CASES, ids=[c[0] for c in CASES])
def test_factorial(name, n, want):
    assert factorial(n) == want
    assert factorial_loop(n) == want
    assert factorial_recursive(n) == want


def test_negative_raises():
    with pytest.raises(ValueError):
        factorial(-1)
    with pytest.raises(ValueError):
        factorial_loop(-1)
