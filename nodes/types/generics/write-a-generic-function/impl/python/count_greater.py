# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-generic-function/.

from collections.abc import Iterable
from typing import Protocol, Self


class Comparable(Protocol):
    """A type whose values can be ordered with <."""

    def __lt__(self, other: Self, /) -> bool: ...


def count_greater[T: Comparable](items: Iterable[T], limit: T) -> int:
    """Return how many items are greater than limit."""
    return sum(1 for item in items if limit < item)
