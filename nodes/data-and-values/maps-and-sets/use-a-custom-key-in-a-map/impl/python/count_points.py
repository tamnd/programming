# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

from collections import Counter
from dataclasses import dataclass


# frozen=True makes the fields read-only and gives the class a __hash__ that
# agrees with its __eq__. order=True gives the order of x, then y.
@dataclass(frozen=True, order=True)
class Point:
    x: int
    y: int


def count_points(points: list[tuple[int, int]]) -> list[tuple[int, int, int]]:
    """Return (x, y, count) for each distinct point, in order of x, then y."""
    counts = Counter(Point(x, y) for x, y in points)
    return [(p.x, p.y, n) for p, n in sorted(counts.items())]
