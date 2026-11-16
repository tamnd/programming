# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-mean-and-median/.

import math
import statistics

import pytest

from meanmedian import mean_and_median, mean_and_median_loop

CASES = [
    ("odd", [3, 1, 2], (2.0, 2.0)),
    ("even", [4, 1, 3, 2], (2.5, 2.5)),
    ("skewed", [1, 2, 3, 4, 100], (22.0, 3.0)),
    ("single", [5], (5.0, 5.0)),
    ("two-items", [1, 2], (1.5, 1.5)),
    ("negative", [-3, -1, -2], (-2.0, -2.0)),
    ("unsorted-even", [10, 2, 8, 4], (6.0, 6.0)),
    ("duplicates", [1, 1, 1, 5], (2.0, 1.0)),
    ("not-an-integer", [1, 2, 2], (1.6666666666666667, 2.0)),
    ("large", [9223372036854775807, 9223372036854775807], (9.223372036854776e+18, 9.223372036854776e+18)),
]


def close(got, want):
    return math.isclose(got, want, rel_tol=1e-12, abs_tol=1e-9)


@pytest.mark.parametrize(("name", "items", "want"), CASES, ids=[c[0] for c in CASES])
def test_mean_and_median(name, items, want):
    before = list(items)
    for f in (mean_and_median, mean_and_median_loop):
        mean, median = f(items)
        assert close(mean, want[0]) and close(median, want[1])
    assert items == before


def test_empty_raises():
    with pytest.raises(statistics.StatisticsError):
        mean_and_median([])
