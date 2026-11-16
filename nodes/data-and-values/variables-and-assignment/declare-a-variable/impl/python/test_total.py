# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-variable/.

import pytest

from total import total

CASES = [
    ("one-two-three", [1, 2, 3], 6),
    ("four-values", [10, 20, 30, 40], 100),
    ("mixed-signs", [5, -2, 7], 10),
    ("empty", [], 0),
    ("single", [42], 42),
    ("all-negative", [-1, -2, -3], -6),
    ("zeros", [0, 0, 0], 0),
    ("cancel-out", [7, -7], 0),
    ("past-32-bits", [4000000000, 5000000000], 9000000000),
    ("eight-values", [1, 1, 1, 1, 1, 1, 1, 1], 8),
]


@pytest.mark.parametrize(("name", "values", "want"), CASES, ids=[c[0] for c in CASES])
def test_total(name, values, want):
    assert total(values) == want
