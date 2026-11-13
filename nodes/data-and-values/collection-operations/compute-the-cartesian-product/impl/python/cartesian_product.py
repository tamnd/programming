# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-cartesian-product/.

from itertools import product


def cartesian_product(lists: list[list[int]]) -> list[list[int]]:
    """Return each tuple that takes one item from each list, as a list, in nested-loop order."""
    # The star passes each inner list as one argument.
    return [list(t) for t in product(*lists)]
