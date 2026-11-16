# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

import pytest

from sumprod import sum_and_product, sum_and_product_loop

CASES = [
    ("small", [1, 2, 3, 4], (10, 24)),
    ("mixed", [3, -2, 5], (6, -30)),
    ("empty", [], (0, 1)),
    ("single", [7], (7, 7)),
    ("with-zero", [5, 0, 9], (14, 0)),
    ("two-negatives", [-2, -3], (-5, 6)),
    ("three-negatives", [-1, -2, -3], (-6, -6)),
    ("ones", [1, 1, 1], (3, 1)),
    ("large-product", [3037000499, 3037000499], (6074000998, 9223372030926249001)),
    ("to-the-limit", [9223372036854775806, 1], (9223372036854775807, 9223372036854775806)),
    ("beyond-double", [9007199254740993], (9007199254740993, 9007199254740993)),
    ("min", [-9223372036854775808], (-9223372036854775808, -9223372036854775808)),
]


@pytest.mark.parametrize(("name", "items", "want"), CASES, ids=[c[0] for c in CASES])
def test_sum_and_product(name, items, want):
    assert sum_and_product(items) == want
    assert sum_and_product_loop(items) == want
