# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-method-on-a-type/.

from dataclasses import dataclass


@dataclass(frozen=True)
class Point:
    """A point on a grid. The dataclass writes __init__, __repr__ and __eq__."""

    x: int
    y: int

    def translate(self, dx, dy):
        """Return the point moved by (dx, dy). The point itself does not change."""
        return Point(self.x + dx, self.y + dy)

    def manhattan(self, other):
        """Return the grid distance |dx| + |dy| to another point."""
        return abs(self.x - other.x) + abs(self.y - other.y)


ORIGIN = Point(0, 0)


def point_report(x, y, dx, dy):
    p = Point(x, y)
    q = p.translate(dx, dy)
    return [q.x, q.y, p.manhattan(q), q.manhattan(ORIGIN)]
