# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

import itertools


def permutations(items: list[int]) -> list[list[int]]:
    """Return each order of the items, in the lexicographic order of the positions."""
    return [list(p) for p in itertools.permutations(items)]
