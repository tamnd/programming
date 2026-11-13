# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/flatten-a-nested-list/.

from itertools import chain


def flatten(lists: list[list[int]]) -> list[int]:
    """Return the items of the inner lists, one level deep, in order."""
    return list(chain.from_iterable(lists))
