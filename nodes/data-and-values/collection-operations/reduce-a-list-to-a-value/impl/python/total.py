# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

from functools import reduce
from operator import add


def total(items: list[int]) -> int:
    """Return the sum of items. The sum of an empty list is 0."""
    # The initial value 0 is the result for an empty list.
    return reduce(add, items, 0)
