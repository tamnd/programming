# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-mean-and-median/.

import statistics


def mean_and_median(items: list[int]) -> tuple[float, float]:
    """Return the mean and the median of a non-empty list.

    statistics.mean and statistics.median raise StatisticsError for an
    empty list. Neither function changes the list.
    """
    return float(statistics.mean(items)), float(statistics.median(items))


def mean_and_median_loop(items: list[int]) -> tuple[float, float]:
    """Return the same pair with a sum and a sorted copy."""
    n = len(items)
    s = sorted(items)
    mid = n // 2
    median = s[mid] if n % 2 == 1 else (s[mid - 1] + s[mid]) / 2
    return sum(items) / n, float(median)
