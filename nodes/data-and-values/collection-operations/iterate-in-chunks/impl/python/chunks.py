# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-in-chunks/.

from itertools import batched


def chunks(items: list[int], size: int) -> list[list[int]]:
    """Return the chunks of items with size items each; the last chunk can be shorter."""
    # batched gives tuples, so make a list of each chunk.
    return [list(b) for b in batched(items, size)]
