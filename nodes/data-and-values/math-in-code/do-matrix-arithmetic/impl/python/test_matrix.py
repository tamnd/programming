# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-matrix-arithmetic/.

import copy

import pytest

from matrix import matrix_multiply, transpose

CASES = [
    ("two-by-two", [[1, 2], [3, 4]], [[5, 6], [7, 8]], [[19, 22], [43, 50]]),
    ("two-by-three-times-three-by-two", [[1, 2, 3], [4, 5, 6]], [[7, 8], [9, 10], [11, 12]], [[58, 64], [139, 154]]),
    ("one-by-one", [[3]], [[4]], [[12]]),
    ("row-times-column", [[1, 2, 3]], [[4], [5], [6]], [[32]]),
    ("column-times-row", [[1], [2], [3]], [[4, 5, 6]], [[4, 5, 6], [8, 10, 12], [12, 15, 18]]),
    ("identity", [[1, 0], [0, 1]], [[5, 6], [7, 8]], [[5, 6], [7, 8]]),
    ("other-order", [[5, 6], [7, 8]], [[1, 2], [3, 4]], [[23, 34], [31, 46]]),
    ("zero-matrix", [[0, 0], [0, 0]], [[1, 2], [3, 4]], [[0, 0], [0, 0]]),
    ("negative-numbers", [[-1, 2], [3, -4]], [[2, 0], [1, -1]], [[0, -2], [2, 4]]),
    ("beyond-32-bit", [[100000, 0], [0, 100000]], [[100000], [3]], [[10000000000], [300000]]),
]


@pytest.mark.parametrize(("name", "a", "b", "want"), CASES, ids=[c[0] for c in CASES])
def test_matrix_multiply(name, a, b, want):
    before = copy.deepcopy((a, b))
    assert matrix_multiply(a, b) == want
    assert (a, b) == before


def test_transpose():
    assert transpose([[1, 2, 3], [4, 5, 6]]) == [[1, 4], [2, 5], [3, 6]]
    assert transpose([[7]]) == [[7]]


def test_sizes_must_match():
    with pytest.raises(ValueError):
        matrix_multiply([[1, 2]], [[1, 2]])
