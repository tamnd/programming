# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

import itertools


def combinations(items: list[int], k: int) -> list[list[int]]:
    """Return each choice of k items, in the lexicographic order of the positions."""
    return [list(c) for c in itertools.combinations(items, k)]
